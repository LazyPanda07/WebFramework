#pragma once

#include "JSONParser.h"

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

	public:
		static AdditionalServerSettings createSettings(const json::JSONParser& parser);
	};
}
