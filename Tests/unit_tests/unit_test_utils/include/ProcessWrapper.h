#pragma once

#include <string>
#include <filesystem>
#include <format>
#include <iostream>

#include <reproc++/run.hpp>

#include "UnitTestUtils.h"

namespace unit_test_utils
{
	class ProcessWrapper
	{
	public:
		static ProcessWrapper runDefaultHttpsServer();

	private:
		reproc::process process;

	public:
		template<typename... Args>
		ProcessWrapper(Args&&... args);

		ProcessWrapper(ProcessWrapper&& other) noexcept = default;

		ProcessWrapper& operator =(ProcessWrapper&& other) noexcept = default;

		~ProcessWrapper();
	};
}

namespace unit_test_utils
{
	template<typename... Args>
	ProcessWrapper::ProcessWrapper(Args&&... args)
	{
		if constexpr (sizeof...(Args) == 1)
		{
			using ActualT = std::remove_cvref_t<decltype(args)...>;

			if constexpr (std::same_as<std::vector<std::string>, ActualT>)
			{
				std::vector<std::string> temp(args...);

				std::error_code errorCode = process.start(reproc::arguments(temp));

				if (errorCode)
				{
					std::string argumentsSingleLine;

					for (const std::string& arg : temp)
					{
						argumentsSingleLine += arg + ' ';
					}

					std::cerr << argumentsSingleLine << std::endl;

					throw std::runtime_error(std::format("Error while creating new process: {}", errorCode.message()));
				}
			}
			else
			{
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
		else
		{
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
}
