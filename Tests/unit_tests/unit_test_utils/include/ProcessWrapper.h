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
	private:
		template<typename... Args>
		void runProcessFromStrings(Args&&... args);

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
	void ProcessWrapper::runProcessFromStrings(Args&&... args)
	{
		std::vector<std::string> arguments({ convertToString(std::forward<Args>(args))... });
		std::error_code error = process.start(reproc::arguments(arguments));

		if (error)
		{
			std::string argumentsSingleLine;

			for (const std::string& arg : arguments)
			{
				argumentsSingleLine += arg + ' ';
			}

			std::cerr << argumentsSingleLine << std::endl;

			throw std::runtime_error(std::format("Error while creating new process: {}", error.message()));
		}
	}

	template<typename... Args>
	ProcessWrapper::ProcessWrapper(Args&&... args)
	{
		constexpr size_t outputBufferSize = 256;

		if constexpr (sizeof...(Args) == 1)
		{
			using FirstArg = std::tuple_element_t<0, std::tuple<Args...>>;
			using ActualT = std::remove_cvref_t<FirstArg>;

			if constexpr (std::same_as<std::vector<std::string>, ActualT>)
			{
				std::vector<std::string> temp(args...);

				std::error_code error = process.start(reproc::arguments(temp));

				if (error)
				{
					std::string argumentsSingleLine;

					for (const std::string& arg : temp)
					{
						argumentsSingleLine += arg + ' ';
					}

					std::cerr << argumentsSingleLine << std::endl;

					throw std::runtime_error(std::format("Error while creating new process: {}", error.message()));
				}
			}
			else
			{
				this->runProcessFromStrings(std::forward<Args>(args)...);
			}
		}
		else
		{
			this->runProcessFromStrings(std::forward<Args>(args)...);
		}

		std::vector<uint8_t> buffer(outputBufferSize, 0);

		while (true)
		{
			auto [size, error] = process.read(reproc::stream::out, buffer.data(), buffer.size());

			if (error)
			{
				throw std::runtime_error(std::format("Error while running new process: {}", error.message()));
			}

			if (size)
			{
				break;
			}

			// TODO: timeout

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}
