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
		pybind11::module_ api;
		bool called;

	public:
		PythonRuntime();

		void initializeWebFramework(std::string_view libraryPath) override;

		~PythonRuntime();
	};
}

#endif
