#include "LocalizationExecutor.h"

#include <Utility/WebFrameworkLocalization.hpp>

void LocalizationExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	response.setBody
	(
		framework::JsonBuilder().append
		(
			"result",
			framework::utility::getLocalizedString("LocalizationData", "key", request.getJson().get<std::string>("language"))
		)
	);
}

DEFINE_EXECUTOR(LocalizationExecutor)
