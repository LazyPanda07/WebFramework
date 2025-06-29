#include "AdditionalServerSettings.h"

#include "Framework/WebFrameworkConstants.h"
#include "WebFrameworkCoreConstants.h"

using namespace std;

namespace framework::utility
{
	AdditionalServerSettings::AdditionalServerSettings() :
		largeBodySizeThreshold(json_settings_values::largeBodySizeThresholdValue),
		largeBodyPacketSize(json_settings_values::largeBodyPacketSizeValue)
	{

	}

	AdditionalServerSettings AdditionalServerSettings::createSettings(const json::JSONParser& parser, const filesystem::path& basePath)
	{
		AdditionalServerSettings result;

		parser.tryGetString(json_settings::userAgentFilterKey, result.userAgentFilter);
		parser.tryGetUnsignedInt(json_settings::largeBodySizeThresholdKey, result.largeBodySizeThreshold);
		parser.tryGetUnsignedInt(json_settings::largeBodyPacketSizeKey, result.largeBodyPacketSize);
		parser.tryGetUnsignedInt(json_settings::cachingSize, result.cachingSize);

		{
			filesystem::path temp = parser.getString(json_settings::assetsPathKey);

			result.assetsPath = temp.is_absolute() ? temp.string() : (basePath / temp).string();
		}

		{
			filesystem::path temp = parser.getString(json_settings::templatesPathKey);

			result.templatesPath = temp.is_absolute() ? temp.string() : (basePath / temp).string();
		}

		return result;
	}
}
