#pragma once

#include <Executors/BaseExecutor.h>

#ifdef __WITH_PYTHON_EXECUTORS__

#include <pybind11/embed.h>

namespace framework
{
	class PythonExecutor : public BaseExecutor
	{
	private:
		void processMethod(std::string_view methodName, interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response);

	private:
		pybind11::object* implementation;

	public:
		PythonExecutor(pybind11::object* implementation);

		PythonExecutor(const PythonExecutor&) = delete;

		PythonExecutor(PythonExecutor&& other) noexcept;

		PythonExecutor& operator =(const PythonExecutor&) = delete;

		PythonExecutor& operator =(PythonExecutor&& other) noexcept;

		void init(const utility::JSONSettingsParser::ExecutorSettings& settings) override;

		void doGet(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doPost(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doHead(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doPut(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doDelete(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doPatch(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doOptions(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doTrace(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doConnect(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~PythonExecutor();
	};
}

#endif
