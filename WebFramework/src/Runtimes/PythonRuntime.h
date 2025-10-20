#pragma once

#include "Runtime.h"

#ifdef __WITH_PYTHON_EXECUTORS__

#include <pybind11/embed.h>

namespace framework::runtime
{
	class PythonRuntime : public Runtime
	{
	private:
		static void loadSymbols();

	private:
		std::unique_ptr<pybind11::scoped_interpreter> guard;
		pybind11::module_ api;
		bool called;

	public:
		PythonRuntime();

		PythonRuntime(const PythonRuntime&) = delete;

		PythonRuntime(PythonRuntime&& other) noexcept;

		PythonRuntime& operator =(const PythonRuntime&) = delete;

		PythonRuntime& operator =(PythonRuntime&& other) noexcept;

		void finishInitialization() override;

		void* createExecutorSettings(const void* implementation) const override;

		void* createHTTPRequest(framework::interfaces::IHTTPRequest* request) const override;

		void* createHTTPResponse(framework::interfaces::IHTTPResponse* response) const override;

		void initializeWebFramework(std::string_view libraryPath) override;

		std::any getClass(std::string_view className, const utility::LoadSource& source) const override;

		~PythonRuntime() = default;
	};
}

#endif
