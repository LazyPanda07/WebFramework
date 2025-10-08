#pragma once

#ifdef __WITH_PYTHON_EXECUTORS__
#include <Executors/BaseExecutor.h>

#include <pybind11/embed.h>

namespace framework
{
	class PythonExecutor : public BaseExecutor
	{
	private:
		pybind11::object* implementation;

	public:
		PythonExecutor(void* implementation);

		~PythonExecutor();
	};
}

#endif
