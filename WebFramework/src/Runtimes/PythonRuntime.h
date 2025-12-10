#pragma once

#include "Runtime.h"

#include <Strings.h>

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
		::utility::strings::string_based_unordered_map<py::object> classes;
		pybind11::module_ api;
		bool called;

	public:
		PythonRuntime();

		PythonRuntime(const PythonRuntime&) = delete;

		PythonRuntime(PythonRuntime&& other) noexcept;

		PythonRuntime& operator =(const PythonRuntime&) = delete;

		PythonRuntime& operator =(PythonRuntime&& other) noexcept;

		std::optional<py::object> getClass(std::string_view className, const utility::LoadSource& source) const;

		void finishInitialization() override;

		bool loadExecutor(std::string_view name, const utility::LoadSource& source) override;

		std::unique_ptr<BaseExecutor> createExecutor(std::string_view name) const override;

		void* createExecutorSettings(const void* implementation) const override;

		void* createHTTPRequest(framework::interfaces::IHTTPRequest* request) const override;

		void* createHTTPResponse(framework::interfaces::IHTTPResponse* response) const override;

		void initializeWebFramework(std::string_view libraryPath) override;

		std::optional<std::string> loadSource(std::string_view pathToSource, utility::LoadSource& source) override;

		~PythonRuntime() = default;
	};
}

#endif
