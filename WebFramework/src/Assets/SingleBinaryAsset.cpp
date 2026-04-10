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

	bool SingleBinaryAsset::Asset::operator ==(const Asset& other) const noexcept
	{
		return path == other.path;
	}

	size_t SingleBinaryAsset::AssetHash::operator ()(const Asset& other) const noexcept
	{
		return std::hash<std::string>()(other.path.string());
	}

	SingleBinaryAsset::SingleBinaryAssetHeader SingleBinaryAsset::parseHeader(const std::filesystem::path& asset, std::ifstream& stream)
	{
		static framework::asset::SingleBinaryAsset::SingleBinaryAssetHeader defaultHeader;

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

	SingleBinaryAsset::SingleBinaryAsset() :
		version(SingleBinaryAssetHeader::currentVersion)
	{
		
	}

	void SingleBinaryAsset::load(const std::filesystem::path& asset)
	{
		std::ifstream stream;
		SingleBinaryAssetHeader header = SingleBinaryAsset::parseHeader(asset, stream);
	}
}
