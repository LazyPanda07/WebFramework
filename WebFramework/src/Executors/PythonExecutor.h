#pragma once

#include <Executors/BaseExecutor.h>

#ifdef __WITH_PYTHON_EXECUTORS__

#include <pybind11/embed.h>

namespace framework
{
	class PythonExecutor : public BaseExecutor
	{
	private:
		void processMethod(std::string_view methodName, HTTPRequestExecutors& request, HTTPResponseExecutors& response);

	private:
		pybind11::object* implementation;

	public:
		PythonExecutor(pybind11::object* implementation);

		PythonExecutor(const PythonExecutor&) = delete;

		PythonExecutor(PythonExecutor&& other) noexcept;

		PythonExecutor& operator =(const PythonExecutor&) = delete;

		PythonExecutor& operator =(PythonExecutor&& other) noexcept;

		void init(const utility::JSONSettingsParser::ExecutorSettings& settings) override;

		void doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~PythonExecutor();
	};
}

#endif
