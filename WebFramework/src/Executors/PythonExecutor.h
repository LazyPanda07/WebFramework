#pragma once

#include <Executors/BaseExecutor.h>

#include <pybind11/embed.h>

namespace framework
{
	class PythonExecutor : public BaseExecutor
	{
	private:
		

	public:
		PythonExecutor();

		~PythonExecutor();
	};
}
