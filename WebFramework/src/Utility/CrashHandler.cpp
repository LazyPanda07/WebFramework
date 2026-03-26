#include "Utility/CrashHandler.h"

#ifdef __WITH_STACKTRACE__

#include <boost/stacktrace.hpp>

#include "Utility/Utils.h"

namespace framework::utility
{
	void CrashHandler::printStacktrace()
	{
		std::ostringstream stacktrace;

		stacktrace << boost::stacktrace::stacktrace();

		if (Log::isValid())
		{
			Log::error<logging::message::crashMessage, logging::category::crashHandler>(stacktrace.str());
		}
		else
		{
			std::cerr << stacktrace.str() << std::endl;
		}
	}

#ifdef __LINUX__
	void CrashHandler::callback(int signal)
	{
		CrashHandler::printStacktrace();

		_Exit(128 + signal);
	}
#else
	LONG CrashHandler::callback(EXCEPTION_POINTERS* information)
	{
		if (information->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
		{
			CrashHandler::printStacktrace();

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
#else
		AddVectoredExceptionHandler(1, handlerFunction);
#endif
	}
}

#endif
