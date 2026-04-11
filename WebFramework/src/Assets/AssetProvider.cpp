#include "Assets/AssetProvider.h"

namespace framework::asset
{
	AssetProvider::AssetProvider(const std::filesystem::path& assetsPath, std::shared_ptr<threading::ThreadPool> threadPool) :
		assetsPath(assetsPath),
		fileManager(file_manager::FileManager::getInstance(threadPool))
	{

	}

	void AssetProvider::getAsset(std::string_view filePath, std::string& result)
	{
		std::filesystem::path assetFilePath(assetsPath / filePath);

		this->getAsset(assetFilePath, result);
	}
}
