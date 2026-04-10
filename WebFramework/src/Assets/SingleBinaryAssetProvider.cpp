#include "Assets/SingleBinaryAssetProvider.h"

namespace framework::asset
{
	void SingleBinaryAssetProvider::getAsset(const std::filesystem::path& filePath, std::string& result)
	{

	}

	SingleBinaryAssetProvider::SingleBinaryAssetProvider(const std::filesystem::path assetsPath, std::shared_ptr<threading::ThreadPool> threadPool, const std::filesystem::path& binaryAssetPath) :
		AssetProvider(assetsPath, threadPool),
		binaryAssetPath(assetsPath / binaryAssetPath)
	{

	}

	bool SingleBinaryAssetProvider::exists(const std::filesystem::path& filePath) const
	{
		return true;
	}

	const std::filesystem::path& SingleBinaryAssetProvider::getPathToAsset() const
	{
		return binaryAssetPath;
	}
}
