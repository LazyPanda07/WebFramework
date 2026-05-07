#pragma once

#include <FileManager.h>

namespace framework::asset
{
	class AssetProvider
	{
	protected:
		const std::filesystem::path assetsPath;
		file_manager::FileManager& fileManager;

	public:
		AssetProvider(const std::filesystem::path& assetsPath, std::shared_ptr<threading::ThreadPool> threadPool);

		virtual bool exists(const std::filesystem::path& filePath) const = 0;

		virtual void getAsset(std::string_view filePath, std::string& result) = 0;

		virtual std::unique_ptr<std::istream> getAssetStream(std::string_view filePath) = 0;

		virtual const std::filesystem::path& getPathToAsset() const = 0;

		virtual ~AssetProvider() = default;
	};
}
