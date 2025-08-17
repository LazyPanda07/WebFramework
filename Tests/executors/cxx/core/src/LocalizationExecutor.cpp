#include "LocalizationExecutor.h"

#include <Utility/WebFrameworkLocalization.hpp>

void LocalizationExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	response.setBody
	(
		framework::JSONBuilder().append
		(
			"result",
			framework::utility::getLocalizedString("LocalizationData", "key", request.getJSON().get<std::string>("language"))
		)
	);
}

DECLARE_EXECUTOR(LocalizationExecutor)
