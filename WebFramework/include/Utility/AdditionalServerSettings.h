#pragma once

#include <filesystem>

#include <JsonParser.h>

namespace framework::utility
{
	struct AdditionalServerSettings
	{
	private:
		AdditionalServerSettings();

	public:
		std::string userAgentFilter;
		size_t largeBodySizeThreshold;
		size_t largeBodyPacketSize;
		std::filesystem::path assetsPath;
		std::filesystem::path templatesPath;
		uint64_t cachingSize;

	public:
		static AdditionalServerSettings createSettings(const json::JsonParser& parser, const std::filesystem::path& basePath);

	public:
		~AdditionalServerSettings() = default;
	};
}
