#include "PythonExecutor.h"

#include <Log.h>

#ifdef __WITH_PYTHON_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/PythonRuntime.h"

namespace py = pybind11;

namespace framework
{
	PythonExecutor::PythonExecutor(void* implementation) :
		implementation(static_cast<py::object*>(implementation))
	{

	}

	void PythonExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void PythonExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		try
		{
			py::gil_scoped_acquire gil;
			const runtime::PythonRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>();
			std::unique_ptr<py::object> pyRequest(static_cast<py::object*>(runtime.createHTTPRequest(request.getImplementation())));
			std::unique_ptr<py::object> pyResponse(static_cast<py::object*>(runtime.createHTTPResponse(response.getImplementation())));

			implementation->attr("do_get")(*pyRequest, *pyResponse);
		}
		catch (const py::error_already_set& e)
		{
			if (Log::isValid())
			{
				Log::error("Serve error: {}", "LogPythonExecutor", e.what());
			}
		}
	}

	void PythonExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void PythonExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void PythonExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void PythonExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void PythonExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void PythonExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void PythonExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void PythonExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

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
	}
}

#endif
