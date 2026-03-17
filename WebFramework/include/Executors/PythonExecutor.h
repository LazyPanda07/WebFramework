#pragma once

#include <Executors/Executor.h>

#ifdef __WITH_PYTHON_EXECUTORS__

#include <pybind11/embed.h>

namespace framework
{
	class PythonExecutor : public Executor
	{
	private:
		void processMethod(std::string_view methodName, interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

	private:
		pybind11::object* implementation;

	public:
		PythonExecutor(pybind11::object* implementation);

		PythonExecutor(const PythonExecutor&) = delete;

		PythonExecutor(PythonExecutor&& other) noexcept;

		PythonExecutor& operator =(const PythonExecutor&) = delete;

		PythonExecutor& operator =(PythonExecutor&& other) noexcept;

		void init(const utility::JSONSettingsParser::ExecutorSettings& settings) override;

		void doGet(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doPost(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doHead(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doPut(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doDelete(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doPatch(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doOptions(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doTrace(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doConnect(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~PythonExecutor();
	};
}

#endif
