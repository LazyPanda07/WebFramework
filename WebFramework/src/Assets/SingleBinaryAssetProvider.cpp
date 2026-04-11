#include "Assets/SingleBinaryAssetProvider.h"

namespace framework::asset
{
	void SingleBinaryAssetProvider::getAsset(const std::filesystem::path& filePath, std::string& result)
	{
		if (asset.isFullyLoad())
		{
			result = asset[filePath];
		}
		else
		{
			file_manager::Cache& cache = fileManager.getCache();

			if (cache.contains(filePath))
			{
				result = cache[filePath];
			}
			else
			{
				result = asset[filePath];

				if (result.size())
				{
					cache.appendCache(filePath, result);
				}
			}
		}
	}

	SingleBinaryAssetProvider::SingleBinaryAssetProvider(const std::filesystem::path assetsPath, std::shared_ptr<threading::ThreadPool> threadPool, const std::filesystem::path& binaryAssetPath, bool fullyLoad) :
		AssetProvider(assetsPath, threadPool),
		asset(assetsPath / binaryAssetPath, fullyLoad)
	{

	}

	bool SingleBinaryAssetProvider::exists(const std::filesystem::path& filePath) const
	{
		return asset.exists(filePath);
	}

	const std::filesystem::path& SingleBinaryAssetProvider::getPathToAsset() const
	{
		return asset.getAsset();
	}
}
