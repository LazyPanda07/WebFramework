#include "PythonExecutor.h"

#include <Log.h>

#ifdef __WITH_PYTHON_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/PythonRuntime.h"

namespace py = pybind11;

namespace framework
{
	void PythonExecutor::processMethod(std::string_view methodName, HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		py::gil_scoped_acquire gil;
		const runtime::PythonRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>();
		std::unique_ptr<py::object> pyRequest(static_cast<py::object*>(runtime.createHTTPRequest(request.getImplementation())));
		std::unique_ptr<py::object> pyResponse(static_cast<py::object*>(runtime.createHTTPResponse(response.getImplementation())));

		implementation->attr(methodName.data())(*pyRequest, *pyResponse);
	}

	PythonExecutor::PythonExecutor(void* implementation) :
		implementation(static_cast<py::object*>(implementation))
	{

	}

	void PythonExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{
		
	}

	void PythonExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->processMethod("do_get", request, response);
	}

	void PythonExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->processMethod("do_post", request, response);
	}

	void PythonExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->processMethod("do_head", request, response);
	}

	void PythonExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->processMethod("do_put", request, response);
	}

	void PythonExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->processMethod("do_delete", request, response);
	}

	void PythonExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->processMethod("do_patch", request, response);
	}

	void PythonExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->processMethod("do_options", request, response);
	}

	void PythonExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->processMethod("do_trace", request, response);
	}

	void PythonExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->processMethod("do_connect", request, response);
	}

	utility::ExecutorType PythonExecutor::getType() const
	{
		return utility::ExecutorType::stateless;
	}

	void PythonExecutor::destroy()
	{

	}

	PythonExecutor::~PythonExecutor()
	{
		py::gil_scoped_acquire gil;

		delete implementation;

		implementation = nullptr;
	}
}

#endif
