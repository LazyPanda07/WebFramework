#include "Assets/DefaultAssetProvider.h"

namespace framework::asset
{
	void DefaultAssetProvider::readFile(std::string& result, std::unique_ptr<file_manager::ReadFileHandle>&& handle)
	{
		result = handle->readAllData();
	}

	void DefaultAssetProvider::getAsset(const std::filesystem::path& filePath, std::string& result)
	{
		fileManager.readBinaryFile(filePath, std::bind(&DefaultAssetProvider::readFile, std::ref(result), std::placeholders::_1));
	}

	DefaultAssetProvider::DefaultAssetProvider(const std::filesystem::path& assetsPath, std::shared_ptr<threading::ThreadPool> threadPool) :
		AssetProvider(assetsPath, threadPool)
	{

	}

	bool DefaultAssetProvider::exists(const std::filesystem::path& filePath) const
	{
		return fileManager.exists(assetsPath / filePath);
	}

	const std::filesystem::path& DefaultAssetProvider::getPathToAsset() const
	{
		return assetsPath;
	}
}
