#include "LocalizationExecutor.h"

#include "MultiLocalizationManager.h"

void LocalizationExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody
	(
		framework::JSONBuilder().append
		(
			"result",
			localization::MultiLocalizationManager::getManager().getLocalizedString("LocalizationData", "key", request.getJSON().get<std::string>("language"))
		)
	);
}

DECLARE_EXECUTOR(LocalizationExecutor)
