#include "Assets/AssetProvider.h"

namespace framework::asset
{
	AssetProvider::AssetProvider(const std::filesystem::path& assetsPath, std::shared_ptr<threading::ThreadPool> threadPool) :
		assetsPath(assetsPath),
		fileManager(file_manager::FileManager::getInstance(threadPool))
	{

	}
}
