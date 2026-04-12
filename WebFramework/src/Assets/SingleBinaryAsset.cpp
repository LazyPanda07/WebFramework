#include "Assets/SingleBinaryAsset.h"

#include "Utility/Utils.h"

class PartialFileBuffer : public std::streambuf
{
private:
	using off_type = std::streambuf::off_type;

private:
	std::ifstream stream;
	off_type offset;
	std::streamsize size;
	size_t currentSize;

private:
	std::streamsize xsgetn(char* data, std::streamsize dataSize) override;

	std::streambuf::pos_type seekoff(std::streambuf::off_type off, std::ios_base::seekdir way, std::ios_base::openmode which) override;

public:
	PartialFileBuffer(std::ifstream&& stream, off_type offset, std::streamsize size);

	~PartialFileBuffer() = default;
};

class ReadOnlyBuffer : public std::streambuf
{
private:
	using off_type = std::streambuf::off_type;

private:
	std::string_view data;
	off_type offset;
	std::streamsize size;
	size_t currentSize;

private:
	std::streamsize xsgetn(char* data, std::streamsize dataSize) override;

	std::streambuf::pos_type seekoff(std::streambuf::off_type off, std::ios_base::seekdir way, std::ios_base::openmode which) override;

public:
	ReadOnlyBuffer(std::string_view data, off_type offset, std::streamsize size);

	~ReadOnlyBuffer() = default;
};

namespace framework::asset
{
	SingleBinaryAsset::SingleBinaryAssetHeader::SingleBinaryAssetHeader() :
		version(SingleBinaryAsset::SingleBinaryAssetHeader::currentVersion),
		fileDataSize(0),
		startFileDataOffset(sizeof(*this))
	{

	}

	SingleBinaryAsset::Asset::Asset(uint64_t offset, uint64_t size) :
		offset(offset),
		size(size)
	{

	}

	SingleBinaryAsset::SingleBinaryAssetHeader SingleBinaryAsset::parseHeader(const std::filesystem::path& asset, std::ifstream& stream)
	{
		static SingleBinaryAssetHeader defaultHeader;

		if (!std::filesystem::exists(asset))
		{
			utility::logAndThrowException<logging::message::cantFindFile, logging::category::singleBinaryAsset>(asset.string());
		}

		stream.open(asset, std::ios::binary);

		if (!stream.is_open())
		{
			utility::logAndThrowException<logging::message::cantOpenFile, logging::category::singleBinaryAsset>(asset.string());
		}

		std::remove_const_t<decltype(SingleBinaryAssetHeader::signature)> signature = {};

		stream.read(reinterpret_cast<char*>(&signature), sizeof(signature));

		if (signature != defaultHeader.signature)
		{
			utility::logAndThrowException<logging::message::notASingleBinaryAsset, logging::category::singleBinaryAsset>(asset.string());
		}

		SingleBinaryAssetHeader result{};

		stream.read(reinterpret_cast<char*>(&result.version), sizeof(SingleBinaryAsset::SingleBinaryAssetHeader::version));

		if (result.version != SingleBinaryAssetHeader::currentVersion)
		{
			utility::logAndThrowException<logging::message::notSupportedVersion, logging::category::singleBinaryAsset>(result.version, SingleBinaryAsset::SingleBinaryAssetHeader::currentVersion);
		}

		stream.read(reinterpret_cast<char*>(&result.fileDataSize), sizeof(SingleBinaryAsset::SingleBinaryAssetHeader::fileDataSize));
		stream.read(reinterpret_cast<char*>(&result.startFileDataOffset), sizeof(SingleBinaryAsset::SingleBinaryAssetHeader::startFileDataOffset));

		return result;
	}

	uint64_t SingleBinaryAsset::calculateOffset(uint64_t offset) const noexcept
	{
		return assetData.empty() ?
			startFileDataOffset + offset :
			offset;
	}

	SingleBinaryAsset::SingleBinaryAsset(const std::filesystem::path& asset, bool fullyLoad) :
		asset(asset),
		version(SingleBinaryAssetHeader::currentVersion),
		startFileDataOffset(0)
	{
		std::ifstream stream;
		SingleBinaryAssetHeader header = SingleBinaryAsset::parseHeader(asset, stream);

		version = header.version;
		startFileDataOffset = header.startFileDataOffset;
		fileDataSize = header.fileDataSize;

		while (stream.tellg() != -1 && static_cast<uint64_t>(stream.tellg()) < startFileDataOffset)
		{
			std::string assetName;
			uint64_t assetNameSize = 0;
			uint64_t offset = 0;
			uint64_t size = 0;

			stream.read(reinterpret_cast<char*>(&assetNameSize), sizeof(assetNameSize));

			assetName.resize(assetNameSize);

			stream.read(assetName.data(), assetNameSize);
			stream.read(reinterpret_cast<char*>(&offset), sizeof(offset));
			stream.read(reinterpret_cast<char*>(&size), sizeof(size));

			assets.try_emplace(assetName, offset, size);
		}

		if (fullyLoad)
		{
			assetData.resize(fileDataSize);

			stream.read(assetData.data(), fileDataSize);
		}
	}

	size_t SingleBinaryAsset::size() const noexcept
	{
		return assets.size();
	}

	bool SingleBinaryAsset::exists(const std::filesystem::path& path) const
	{
		return assets.contains(path);
	}

	const std::filesystem::path& SingleBinaryAsset::getAsset() const noexcept
	{
		return asset;
	}

	uint64_t SingleBinaryAsset::getFileDataSize() const noexcept
	{
		return fileDataSize;
	}

	uint64_t SingleBinaryAsset::getStartFileDataOffset() const noexcept
	{
		return startFileDataOffset;
	}

	std::unique_ptr<std::istream> SingleBinaryAsset::getFileStream(const std::filesystem::path& path) const
	{
		auto [offset, size] = assets.at(path);

		offset = this->calculateOffset(offset);

		if (assetData.empty())
		{
			return std::make_unique<std::istream>(new PartialFileBuffer(std::ifstream(asset), offset, size));
		}
		else
		{
			return std::make_unique<std::istream>(new ReadOnlyBuffer(assetData, offset, size));
		}
	}

	std::string SingleBinaryAsset::operator [](const std::filesystem::path& path) const
	{
		if (auto it = assets.find(path); it != assets.end())
		{
			auto [offset, size] = it->second;

			offset = this->calculateOffset(offset);

			if (assetData.empty())
			{
				std::ifstream stream(asset, std::ios::binary);
				std::string result;

				result.resize(size);

				stream.seekg(offset, std::ios::beg);

				stream.read(result.data(), size);

				return result;
			}
			else
			{
				return std::string(assetData.begin() + offset, assetData.begin() + offset + size);
			}
		}

		return "";
	}
}

std::streamsize PartialFileBuffer::xsgetn(char* data, std::streamsize dataSize)
{
	std::streamsize availableBytes = size - currentSize;
	std::streamsize readBytes = std::clamp<std::streamsize>(dataSize, 0, availableBytes);

	if (!readBytes)
	{
		return readBytes;
	}

	stream.read(data, readBytes);

	currentSize += readBytes;

	return readBytes;
}

std::streambuf::pos_type PartialFileBuffer::seekoff(std::streambuf::off_type off, std::ios_base::seekdir way, std::ios_base::openmode which)
{
	switch (way)
	{
	case std::ios_base::beg:
		return stream.seekg(offset + off, way).tellg();

	case std::ios_base::cur:
		return stream.seekg(off, way).tellg();

	case std::ios_base::end:
		return stream.seekg(offset + size - off, std::ios_base::beg).tellg();
	}

	throw std::runtime_error(std::format("Wrong seekdir type: {}", way));

	return 0;
}

PartialFileBuffer::PartialFileBuffer(std::ifstream&& stream, off_type offset, std::streamsize size) :
	stream(std::move(stream)),
	offset(offset),
	size(size),
	currentSize(0)
{
	this->stream.seekg(offset, std::ios::beg);
}

std::streamsize ReadOnlyBuffer::xsgetn(char* data, std::streamsize dataSize)
{
	std::streamsize availableBytes = size - currentSize;
	std::streamsize readBytes = std::clamp<std::streamsize>(dataSize, 0, availableBytes);

	if (!readBytes)
	{
		return readBytes;
	}

	std::memcpy(data, this->data.data() + currentSize + offset, readBytes);

	currentSize += readBytes;

	return readBytes;
}

std::streambuf::pos_type ReadOnlyBuffer::seekoff(std::streambuf::off_type off, std::ios_base::seekdir way, std::ios_base::openmode which)
{
	switch (way)
	{
	case std::ios_base::beg:
		currentSize = off;

		break;

	case std::ios_base::cur:
		currentSize += off;

		break;

	case std::ios_base::end:
		currentSize = size - off;

		break;
	}
	
	return offset + currentSize;
}

ReadOnlyBuffer::ReadOnlyBuffer(std::string_view data, off_type offset, std::streamsize size) :
	data(data),
	offset(offset),
	size(size),
	currentSize(0)
{

}
