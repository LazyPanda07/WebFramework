#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <unordered_set>

namespace framework::asset
{
	class SingleBinaryAsset
	{
	public:
		struct SingleBinaryAssetHeader
		{
			static inline constexpr uint64_t currentVersion = 1;

			const uint64_t signature = 0xA67BE03C83CD02AA;
			uint64_t version;
			uint64_t fileDataSize;
			uint64_t startFileDataOffset;

		public:
			SingleBinaryAssetHeader();

			~SingleBinaryAssetHeader() = default;
		};

		struct Asset
		{
			std::filesystem::path path;
			uint64_t offset;
			uint64_t size;

			bool operator ==(const Asset& other) const noexcept;
		};

		struct AssetHash
		{
			size_t operator ()(const Asset& other) const noexcept;
		};

	private:
		static SingleBinaryAssetHeader parseHeader(const std::filesystem::path& asset, std::ifstream& stream);

	private:
		std::unordered_set<Asset, AssetHash> assets;
		decltype(SingleBinaryAssetHeader::version) version;

	public:
		SingleBinaryAsset();

		void load(const std::filesystem::path& asset);

		~SingleBinaryAsset() = default;
	};
}
