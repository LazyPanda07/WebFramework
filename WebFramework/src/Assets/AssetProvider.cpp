#include "Assets/AssetProvider.h"

#include "Exceptions/FileDoesNotExistException.h"

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

		if (!std::filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		this->getAsset(assetFilePath, result);
	}
}
