#pragma once

#include <Log.h>
#include <IOSocketStream.h>

#include "Framework/WebFrameworkConstants.h"
#include "WebFrameworkConcepts.h"
#include "Exceptions/AlreadyLoggedException.h"

namespace framework::utility
{
	bool isVariableExist(std::string_view key);

	void setEnvironmentVariable(std::string_view key, std::string_view value);

	std::string getEnvironmentVariable(std::string_view key);

	std::string generateRandomString(size_t size);

	template<const auto& Format, const auto& Category, typename... Args>
	[[noreturn]] void logAndThrowException(Args&&... args) requires log_utils::LogFormat<Format, Args...>;

	template<const auto& Category, utility::concepts::StreamOperation OperationT, typename T>
	bool processStreamOperation(streams::IOSocketStream& stream, T& data);
}

namespace framework::utility
{
	template<const auto& Format, const auto& Category, typename... Args>
	[[noreturn]] void logAndThrowException(Args&&... args) requires log_utils::LogFormat<Format, Args...>
	{
		if (Log::isValid())
		{
			Log::error<Format, Category>(args...);
		}

		throw exceptions::AlreadyLoggedException(std::format(Format, std::forward<Args>(args)...));
	}

	template<const auto& Category, utility::concepts::StreamOperation OperationT, typename T>
	bool processStreamOperation(streams::IOSocketStream& stream, T& data)
	{
		try
		{
			OperationT::process(stream, data);

			return true;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error<OperationT::errorName, Category>(e.what());
			}
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::error<OperationT::errorName, Category>("Unexpected error");
			}
		}

		return false;
	}
}
