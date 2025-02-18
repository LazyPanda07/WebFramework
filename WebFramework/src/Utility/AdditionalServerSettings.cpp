#include "AdditionalServerSettings.h"

#include "Import/WebFrameworkConstants.h"

namespace framework::utility
{
	AdditionalServerSettings::AdditionalServerSettings() :
		largeBodySizeThreshold(json_settings_values::largeBodySizeThresholdValue),
		largeBodyPacketSize(json_settings_values::largeBodyPacketSizeValue)
	{

	}

	AdditionalServerSettings AdditionalServerSettings::createSettings(const json::JSONParser& parser)
	{
		AdditionalServerSettings result;

		parser.tryGetString(json_settings::userAgentFilterKey, result.userAgentFilter);
		parser.tryGetUnsignedInt(json_settings::largeBodySizeThresholdKey, result.largeBodySizeThreshold);
		parser.tryGetUnsignedInt(json_settings::largeBodyPacketSizeKey, result.largeBodyPacketSize);

		return result;
	}
}
