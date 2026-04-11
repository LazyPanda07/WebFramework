#pragma once

#include "AssetProvider.h"

#include "SingleBinaryAsset.h"

namespace framework::asset
{
	class SingleBinaryAssetProvider : public AssetProvider
	{
	private:
		SingleBinaryAsset asset;

	private:
		void getAsset(const std::filesystem::path& filePath, std::string& result) override;

	public:
		SingleBinaryAssetProvider(const std::filesystem::path assetsPath, std::shared_ptr<threading::ThreadPool> threadPool, const std::filesystem::path& binaryAssetPath, bool fullyLoad);

		bool exists(const std::filesystem::path& filePath) const override;

		const std::filesystem::path& getPathToAsset() const override;

		~SingleBinaryAssetProvider() = default;

		using AssetProvider::getAsset;
	};
}
