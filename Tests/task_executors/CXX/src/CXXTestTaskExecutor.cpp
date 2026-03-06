#include "CXXTestTaskExecutor.h"

#include <fstream>

void CXXTestTaskExecutor::operator ()(const framework::JsonObject& data)
{
	std::ofstream out("cxx_task_executor.txt");
	std::string message;

	data.tryGet<std::string>("message", message);

	out << message;
}

DEFINE_TASK_EXECUTOR(CXXTestTaskExecutor)
