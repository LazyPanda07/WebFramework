#pragma once

#include "AssetProvider.h"

namespace framework::asset
{
	class DefaultAssetProvider : public AssetProvider
	{
	private:
		static void readFile(std::string& result, std::unique_ptr<file_manager::ReadFileHandle>&& handle);

	private:
		void getAsset(const std::filesystem::path& filePath, std::string& result) override;

	public:
		DefaultAssetProvider(const std::filesystem::path& assetsPath, std::shared_ptr<threading::ThreadPool> threadPool);

		bool exists(const std::filesystem::path& filePath) const override;

		const std::filesystem::path& getPathToAsset() const override;

		~DefaultAssetProvider() = default;

		using AssetProvider::getAsset;
	};
}
