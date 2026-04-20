#include "TokenExecutor.h"

void TokenExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	const framework::JsonObject data = *request.getTokenPayload();

	return response.setBody(data["userName"].get<std::string>());
}
