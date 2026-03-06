#include "CXXTestTaskExecutor.h"

#include <fstream>

void CXXTestTaskExecutor::operator ()(const framework::JsonObject& data)
{
	std::string fileName;
	std::string message;

	data.tryGet<std::string>("fileName", fileName);
	data.tryGet<std::string>("message", message);

	std::ofstream(fileName) << message;
}

DEFINE_TASK_EXECUTOR(CXXTestTaskExecutor)
