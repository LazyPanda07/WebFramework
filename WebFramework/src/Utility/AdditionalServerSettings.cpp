#include "Utility/AdditionalServerSettings.h"

#include <JsonArrayWrapper.h>

#include "Framework/WebFrameworkConstants.h"
#include "WebFrameworkCoreConstants.h"

namespace framework::utility
{
	AdditionalServerSettings::AdditionalServerSettings() :
		largeBodySizeThreshold(json_settings_values::largeBodySizeThresholdValue),
		largeBodyPacketSize(json_settings_values::largeBodyPacketSizeValue),
		cachingSize(0)
	{

	}

	AdditionalServerSettings AdditionalServerSettings::createSettings(const json::JsonParser& parser, const std::filesystem::path& basePath)
	{
		AdditionalServerSettings result;
		std::string value;

		parser.tryGet<size_t>(json_settings::largeBodySizeThresholdKey, result.largeBodySizeThreshold);
		parser.tryGet<size_t>(json_settings::largeBodyPacketSizeKey, result.largeBodyPacketSize);
		parser.tryGet<uint64_t>(json_settings::cachingSize, result.cachingSize);

		if (parser.tryGet<std::string>(json_settings::assetsPathKey, value))
		{
			std::filesystem::path temp(value);

			result.assetsPath = temp.is_absolute() ? temp : (basePath / temp);
		}

		if (parser.tryGet<std::string>(json_settings::templatesPathKey, value))
		{
			std::filesystem::path temp(value);

			result.templatesPath = temp.is_absolute() ? temp : (basePath / temp);
		}

		if (parser.contains<std::string>(json_settings::userAgentFilterKey))
		{
			result.userAgentFilter.emplace_back(parser.get<std::string>(json_settings::userAgentFilterKey));
		}
		else if (parser.contains<std::vector<json::JsonObject>>(json_settings::userAgentFilterKey))
		{
			result.userAgentFilter = json::utility::JsonArrayWrapper(parser.get<std::vector<json::JsonObject>>(json_settings::userAgentFilterKey)).as<std::string>();
		}

		return result;
	}
}
