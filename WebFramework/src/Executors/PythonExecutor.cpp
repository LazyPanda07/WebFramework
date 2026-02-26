#include "Executors/PythonExecutor.h"

#ifdef __WITH_PYTHON_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/PythonRuntime.h"

namespace py = pybind11;

namespace framework
{
	void PythonExecutor::processMethod(std::string_view methodName, interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		py::gil_scoped_acquire gil;
		const runtime::PythonRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>();
		std::unique_ptr<py::object> pyRequest(static_cast<py::object*>(runtime.createHTTPRequest(&request)));
		std::unique_ptr<py::object> pyResponse(static_cast<py::object*>(runtime.createHTTPResponse(&response)));

		implementation->attr(methodName.data())(*pyRequest, *pyResponse);
	}

	PythonExecutor::PythonExecutor(pybind11::object* implementation) :
		implementation(implementation)
	{
		
	}

	PythonExecutor::PythonExecutor(PythonExecutor&& other) noexcept
	{
		(*this) = std::move(other);
	}

	PythonExecutor& PythonExecutor::operator =(PythonExecutor&& other) noexcept
	{
		implementation = other.implementation;

		other.implementation = nullptr;

		return *this;
	}

	void PythonExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{
		py::gil_scoped_acquire gil;
		const runtime::PythonRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>();
		std::unique_ptr<py::object> pyExecutorSettings(static_cast<py::object*>(runtime.createExecutorSettings(&settings)));

		implementation->attr("init")(*pyExecutorSettings);
	}

	void PythonExecutor::doGet(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		this->processMethod("do_get", request, response);
	}

	void PythonExecutor::doPost(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		this->processMethod("do_post", request, response);
	}

	void PythonExecutor::doHead(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		this->processMethod("do_head", request, response);
	}

	void PythonExecutor::doPut(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		this->processMethod("do_put", request, response);
	}

	void PythonExecutor::doDelete(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		this->processMethod("do_delete", request, response);
	}

	void PythonExecutor::doPatch(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		this->processMethod("do_patch", request, response);
	}

	void PythonExecutor::doOptions(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		this->processMethod("do_options", request, response);
	}

	void PythonExecutor::doTrace(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		this->processMethod("do_trace", request, response);
	}

	void PythonExecutor::doConnect(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		this->processMethod("do_connect", request, response);
	}

	utility::ExecutorType PythonExecutor::getType() const
	{
		py::gil_scoped_acquire gil;

		return static_cast<utility::ExecutorType>(implementation->attr("get_type")().cast<int>());
	}

	void PythonExecutor::destroy()
	{
		utility::ExecutorType executorType = this->getType();

		if (executorType == utility::ExecutorType::stateful ||
			executorType == utility::ExecutorType::heavyOperationStateful)
		{
			py::gil_scoped_acquire gil;

			implementation->attr("destroy")();
		}
	}

	PythonExecutor::~PythonExecutor()
	{
		if (implementation)
		{
			py::gil_scoped_acquire gil;

			delete implementation;

			implementation = nullptr;
		}
	}
}

#endif
