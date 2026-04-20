#include "TokenGiverExecutor.h"

#include <Utility/WebFrameworkUtility.hpp>

void TokenGiverExecutor::doPost(framework::HttpRequest& request, framework::HttpResponse& response)
{
	using namespace std::chrono_literals;

	const framework::JsonParser& json = request.getJson();
	framework::JsonBuilder result;
	framework::JsonObject data;

	data["userName"] = json.get<std::string>("userName");

	result["token"] = framework::utility::token::createJwt(data, 60min);

	response.setBody(result);
}
