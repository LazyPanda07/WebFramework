#pragma once

#include "AssetProvider.h"

namespace framework::asset
{
	class DefaultAssetProvider : public AssetProvider
	{
	private:
		static void readFile(std::string& result, std::unique_ptr<file_manager::ReadFileHandle>&& handle);

	public:
		DefaultAssetProvider(const std::filesystem::path& assetsPath, std::shared_ptr<threading::ThreadPool> threadPool);

		bool exists(const std::filesystem::path& filePath) const override;

		void getAsset(std::string_view filePath, std::string& result) override;

		std::unique_ptr<std::istream> getAssetStream(std::string_view filePath) override;

		const std::filesystem::path& getPathToAsset() const override;

		~DefaultAssetProvider() = default;

		using AssetProvider::getAsset;
	};
}
