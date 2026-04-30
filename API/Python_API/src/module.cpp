#include <pybind11/pybind11.h>

#include "Registrars/SqlRegistrar.h"
#include "Registrars/TaskBrokerRegistrar.h"
#include "Registrars/WebFrameworkUtilityRegistrar.h"
#include "Registrars/ExecutorRegistrar.h"
#include "Registrars/WebFrameworkApiRegistrar.h"
#include "Registrars/HttpResponseRegistrar.h"
#include "Registrars/HttpRequestRegistrar.h"
#include "Registrars/LoadBalancerRegistrar.h"
#include "Registrars/WebSocketExecutorRegistrar.h"

namespace py = pybind11;

PYBIND11_MODULE(web_framework_api, m, py::mod_gil_not_used())
{
	m.doc() = "Python API for WebFramework";

	registrar::registerSql(m);

	registrar::registerWebFrameworkApi(m);

	registrar::registerUtility(m); // contains utility function that requires SQL
	
	registrar::registerTaskBrokers(m);

	registrar::registerUtilityStructures(m);

	registrar::registerExecutorSettings(m);

	registrar::registerLoadBalancerHeuristic(m);

	registrar::registerHttpResponse(m);

	registrar::registerHttpRequest(m);

	registrar::registerExecutors(m);

	registrar::registerWebSocketExecutor(m);
}
