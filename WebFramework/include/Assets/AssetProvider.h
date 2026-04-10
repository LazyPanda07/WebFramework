#pragma once

#include <FileManager.h>

namespace framework::asset
{
	class AssetProvider
	{
	protected:
		const std::filesystem::path assetsPath;
		file_manager::FileManager& fileManager;

	protected:
		virtual void getAsset(const std::filesystem::path& filePath, std::string& result) = 0;

	public:
		AssetProvider(const std::filesystem::path& assetsPath, std::shared_ptr<threading::ThreadPool> threadPool);

		void getAsset(std::string_view filePath, std::string& result);

		virtual bool exists(const std::filesystem::path& filePath) const = 0;

		virtual const std::filesystem::path& getPathToAsset() const = 0;

		virtual ~AssetProvider() = default;
	};
}
