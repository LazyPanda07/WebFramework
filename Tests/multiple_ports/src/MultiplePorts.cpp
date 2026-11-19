#include "MultiplePorts.hpp"

void MultiplePorts::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	response.setBody(std::to_string(request.getCurrentPort()));
}

DEFINE_EXECUTOR(MultiplePorts);
