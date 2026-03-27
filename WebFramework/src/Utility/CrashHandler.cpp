#include "Utility/CrashHandler.h"

#ifdef __WITH_STACKTRACE__

#include <boost/stacktrace.hpp>

#include "Utility/Utils.h"

namespace framework::utility
{
	void CrashHandler::printStacktrace(int errorCode)
	{
		std::ostringstream stacktrace;

		stacktrace << boost::stacktrace::stacktrace();

		if (Log::isValid())
		{
			Log::error<logging::message::crashMessage, logging::category::crashHandler>(stacktrace.str(), errorCode);
		}
		else
		{
			std::cerr << stacktrace.str() << std::endl << "Error code: " << errorCode << std::endl;
		}
	}

#ifdef __LINUX__
	void CrashHandler::callback(int signal)
	{
		constexpr int offset = 128;

		CrashHandler::printStacktrace(offset + signal);

		_Exit(offset + signal);
	}
#else
	LONG CrashHandler::callback(EXCEPTION_POINTERS* information)
	{
		if (information->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
		{
			CrashHandler::printStacktrace(information->ExceptionRecord->ExceptionCode);

			return EXCEPTION_EXECUTE_HANDLER;
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
#endif

	CrashHandler::CrashHandler()
	{
		auto handlerFunction = &CrashHandler::callback;

#ifdef __LINUX__
		handler.sa_handler = handlerFunction;

		sigemptyset(&handler.sa_mask);
		
		handler.sa_flags = 0;
		
		sigaction(SIGSEGV, &handler, nullptr);
		sigaction(SIGPIPE, &handler, nullptr);
		sigaction(SIGABRT, &handler, nullptr);
#else
		AddVectoredExceptionHandler(1, handlerFunction);
#endif
	}
}

#endif
