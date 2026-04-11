#include "Assets/SingleBinaryAsset.h"

#include "Utility/Utils.h"

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

	std::string SingleBinaryAsset::operator [](const std::filesystem::path& path) const
	{
		if (auto it = assets.find(path); it != assets.end())
		{
			const auto& [offset, size] = it->second;

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
