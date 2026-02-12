#pragma once

#include <string>
#include <filesystem>
#include <format>
#include <iostream>

#include <reproc++/run.hpp>

namespace process_utils
{
	class ProcessWrapper
	{
	private:
		reproc::process process;

	public:
		template<typename... Args>
		ProcessWrapper(Args&&... args);

		ProcessWrapper(const std::vector<std::string>& args);

		~ProcessWrapper();
	};
}

namespace process_utils
{
	template<typename... Args>
	ProcessWrapper::ProcessWrapper(Args&&... args)
	{
		auto convertToString = []<typename T>(T && value)
		{
			if constexpr (std::same_as<std::filesystem::path, T>)
			{
				return value.string();
			}
			else
			{
				return value;
			}
		};

		std::vector<std::string> arguments({ convertToString(std::forward<Args>(args))... });
		std::error_code errorCode = process.start(reproc::arguments(arguments));

		if (errorCode)
		{
			std::string argumentsSingleLine;

			for (const std::string& arg : arguments)
			{
				argumentsSingleLine += arg + ' ';
			}

			std::cerr << argumentsSingleLine << std::endl;

			throw std::runtime_error(std::format("Error while creating new process: {}", errorCode.message()));
		}
	}
}
