#include "AdditionalServerSettings.h"

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

	AdditionalServerSettings AdditionalServerSettings::createSettings(const json::JSONParser& parser, const std::filesystem::path& basePath)
	{
		AdditionalServerSettings result;
		std::string value;

		parser.tryGetString(json_settings::userAgentFilterKey, result.userAgentFilter);
		parser.tryGetUnsignedInt(json_settings::largeBodySizeThresholdKey, result.largeBodySizeThreshold);
		parser.tryGetUnsignedInt(json_settings::largeBodyPacketSizeKey, result.largeBodyPacketSize);
		parser.tryGetUnsignedInt(json_settings::cachingSize, result.cachingSize);

		if (parser.tryGetString(json_settings::assetsPathKey, value))
		{
			std::filesystem::path temp(value);

			result.assetsPath = temp.is_absolute() ? temp : (basePath / temp);
		}

		if (parser.tryGetString(json_settings::templatesPathKey, value))
		{
			std::filesystem::path temp(value);

			result.templatesPath = temp.is_absolute() ? temp : (basePath / temp);
		}

		return result;
	}
}
