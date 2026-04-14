#include "Assets/DefaultAssetProvider.h"

#include <Exceptions/FileDoesNotExistException.h>

#include "Utility/ReadOnlyBuffer.h"

namespace framework::asset
{
	void DefaultAssetProvider::readFile(std::string& result, std::unique_ptr<file_manager::ReadFileHandle>&& handle)
	{
		result = handle->readAllData();
	}

	DefaultAssetProvider::DefaultAssetProvider(const std::filesystem::path& assetsPath, std::shared_ptr<threading::ThreadPool> threadPool) :
		AssetProvider(assetsPath, threadPool)
	{

	}

	bool DefaultAssetProvider::exists(const std::filesystem::path& filePath) const
	{
		return fileManager.exists(assetsPath / filePath);
	}

	void DefaultAssetProvider::getAsset(std::string_view filePath, std::string& result)
	{
		std::filesystem::path assetFilePath(assetsPath / filePath);

		if (!std::filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		fileManager.readBinaryFile(assetFilePath, std::bind(&DefaultAssetProvider::readFile, std::ref(result), std::placeholders::_1));
	}

	std::unique_ptr<std::istream> DefaultAssetProvider::getAssetStream(std::string_view filePath)
	{
		std::filesystem::path assetFilePath(assetsPath / filePath);

		if (!std::filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		file_manager::Cache& cache = fileManager.getCache();

		if (cache.contains(assetFilePath))
		{
			std::string_view cacheData = cache.getCacheData(assetFilePath);

			return std::make_unique<std::istream>(new utility::ReadOnlyBuffer(cacheData, 0, cacheData.size()));
		}

		return std::make_unique<std::ifstream>(assetFilePath, std::ios::binary);
	}

	const std::filesystem::path& DefaultAssetProvider::getPathToAsset() const
	{
		return assetsPath;
	}
}
