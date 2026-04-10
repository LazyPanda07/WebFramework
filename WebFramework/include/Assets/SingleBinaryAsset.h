#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>

namespace framework::asset
{
	class SingleBinaryAsset
	{
	private:
		struct SingleBinaryAssetHeader
		{
			static inline constexpr uint64_t signature = 0xA67BE03C83CD02AA;
			static inline constexpr uint64_t currentVersion = 1;

			uint64_t version;
			uint64_t fileDataSize;
			uint64_t startFileDataOffset;
		};

	private:
		static SingleBinaryAssetHeader parseHeader(const std::filesystem::path& asset, std::ifstream& assetStream);

	public:
		SingleBinaryAsset(const std::filesystem::path& asset);

		~SingleBinaryAsset() = default;
	};
}
