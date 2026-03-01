#pragma once

#include <filesystem>

#include <JsonObject.h>

namespace framework::utility
{
	struct TaskExecutorsSettings
	{
	public:
		std::filesystem::path sourcePath;
		std::vector<std::string> classNames;
		std::string api;

	public:
		static TaskExecutorsSettings createTaskExecutorsSettings(const json::JsonObject& taskExecutorsArrayElement);
	};
}
