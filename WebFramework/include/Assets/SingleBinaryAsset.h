#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <unordered_map>

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
			uint64_t offset;
			uint64_t size;

		public:
			Asset(uint64_t offset, uint64_t size);
		};

	private:
		static SingleBinaryAssetHeader parseHeader(const std::filesystem::path& asset, std::ifstream& stream);

	private:
		std::unordered_map<std::filesystem::path, Asset> assets;
		std::filesystem::path asset;
		decltype(SingleBinaryAssetHeader::version) version;
		uint64_t startFileDataOffset;
		uint64_t fileDataSize;
		std::string assetData;

	private:
		uint64_t calculateOffset(uint64_t offset) const noexcept;

	public:
		SingleBinaryAsset(const std::filesystem::path& asset, bool fullyLoad);

		size_t size() const noexcept;

		bool exists(const std::filesystem::path& path) const;

		bool isFullyLoad() const noexcept;

		const std::filesystem::path& getAsset() const noexcept;

		uint64_t getFileDataSize() const noexcept;

		uint64_t getStartFileDataOffset() const noexcept;

		std::unique_ptr<std::istream> getFileStream(const std::filesystem::path& path) const;

		std::string operator [](const std::filesystem::path& path) const;

		~SingleBinaryAsset() = default;
	};
}

namespace framework::asset
{
	inline bool SingleBinaryAsset::isFullyLoad() const noexcept
	{
		return !assetData.empty();
	}
}
