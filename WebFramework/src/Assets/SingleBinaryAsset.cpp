#include "Assets/SingleBinaryAsset.h"

#include "Utility/Utils.h"

namespace framework::asset
{
	SingleBinaryAsset::SingleBinaryAssetHeader SingleBinaryAsset::parseHeader(const std::filesystem::path& asset, std::ifstream& assetStream)
	{
		if (!std::filesystem::exists(asset))
		{
			utility::logAndThrowException<logging::message::cantFindFile, logging::category::singleBinaryAsset>(asset.string());
		}

		assetStream.open(asset, std::ios::binary);

		if (!assetStream.is_open())
		{
			utility::logAndThrowException<logging::message::cantOpenFile, logging::category::singleBinaryAsset>(asset.string());
		}

		std::remove_const_t<decltype(SingleBinaryAsset::SingleBinaryAssetHeader::signature)> signature = {};

		assetStream.read(reinterpret_cast<char*>(&signature), sizeof(signature));

		if (signature != SingleBinaryAsset::SingleBinaryAssetHeader::signature)
		{
			utility::logAndThrowException<logging::message::notASingleBinaryAsset, logging::category::singleBinaryAsset>(asset.string());
		}

		SingleBinaryAssetHeader result{};

		assetStream.read(reinterpret_cast<char*>(&result.version), sizeof(SingleBinaryAsset::SingleBinaryAssetHeader::version));

		if (result.version != SingleBinaryAsset::SingleBinaryAssetHeader::currentVersion)
		{
			utility::logAndThrowException<logging::message::notSupportedVersion, logging::category::singleBinaryAsset>(result.version, SingleBinaryAsset::SingleBinaryAssetHeader::currentVersion);
		}

		assetStream.read(reinterpret_cast<char*>(&result.fileDataSize), sizeof(SingleBinaryAsset::SingleBinaryAssetHeader::fileDataSize));
		assetStream.read(reinterpret_cast<char*>(&result.startFileDataOffset), sizeof(SingleBinaryAsset::SingleBinaryAssetHeader::startFileDataOffset));

		return result;
	}

	SingleBinaryAsset::SingleBinaryAsset(const std::filesystem::path& asset)
	{
		std::ifstream assetStream;
		SingleBinaryAssetHeader header = SingleBinaryAsset::parseHeader(asset, assetStream);
	}
}
