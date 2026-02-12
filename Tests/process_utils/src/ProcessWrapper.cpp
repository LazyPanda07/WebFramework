#include "ProcessWrapper.h"

namespace process_utils
{
	ProcessWrapper::ProcessWrapper(const std::vector<std::string>& args)
	{
		std::error_code errorCode = process.start(reproc::arguments(args));

		if (errorCode)
		{
			std::string argumentsSingleLine;

			for (const std::string& arg : args)
			{
				argumentsSingleLine += arg + ' ';
			}

			std::cerr << argumentsSingleLine << std::endl;

			throw std::runtime_error(std::format("Error while creating new process: {}", errorCode.message()));
		}
	}

	ProcessWrapper::~ProcessWrapper()
	{
		process.kill();
	}
}
