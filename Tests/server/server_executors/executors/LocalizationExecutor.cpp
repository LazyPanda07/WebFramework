#include "LocalizationExecutor.h"

#include "MultiLocalizationManager.h"

void LocalizationExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody
	(
		json::JSONBuilder(CP_UTF8).appendString
		(
			"result",
			localization::MultiLocalizationManager::getManager().getLocalizedString("LocalizationData", "key", request.getJSON().getString("language"))
		)
	);
}

DECLARE_EXECUTOR(LocalizationExecutor)
