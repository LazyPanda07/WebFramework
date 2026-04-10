#pragma once

#include "AssetProvider.h"

namespace framework::asset
{
	class SingleBinaryAssetProvider : public AssetProvider
	{
	private:
		std::filesystem::path binaryAssetPath;

	private:
		void getAsset(const std::filesystem::path& filePath, std::string& result) override;

	public:
		SingleBinaryAssetProvider(const std::filesystem::path assetsPath, std::shared_ptr<threading::ThreadPool> threadPool, const std::filesystem::path& binaryAssetPath);

		bool exists(const std::filesystem::path& filePath) const override;

		const std::filesystem::path& getPathToAsset() const override;

		~SingleBinaryAssetProvider() = default;

		using AssetProvider::getAsset;
	};
}
