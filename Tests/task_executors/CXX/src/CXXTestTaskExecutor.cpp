#include "CXXTestTaskExecutor.h"

#include <fstream>

void CXXTestTaskExecutor::operator ()(const framework::JsonObject& data)
{
	constexpr std::string_view header = "From CXXTestTaskExecutor";

	std::string fileName;
	std::string message;

	data.tryGet<std::string>("fileName", fileName);
	data.tryGet<std::string>("message", message);

	std::ofstream(fileName) << header << std::endl << message;
}

DEFINE_TASK_EXECUTOR(CXXTestTaskExecutor)
