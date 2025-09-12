#include "SegfaultHandler.h"

#ifdef __WITH_STACKTRACE__

#include <boost/stacktrace.hpp>

#include "Log.h"

using namespace std;

namespace framework::utility
{
	void SegfaultHandler::printStacktrace()
	{
		ostringstream stacktrace;

		stacktrace << boost::stacktrace::stacktrace();

		if (Log::isValid())
		{
			Log::error("Segmentation fault: {}", "LogSegfaultHandler", stacktrace.str());
		}
		else
		{
			cerr << stacktrace.str() << endl;
		}
	}

#ifdef __LINUX__
	void SegfaultHandler::callback(int signal)
	{
		SegfaultHandler::printStacktrace();

		_Exit(128 + signal);
	}
#else
	LONG SegfaultHandler::callback(EXCEPTION_POINTERS* information)
	{
		if (information->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
		{
			SegfaultHandler::printStacktrace();

			return EXCEPTION_EXECUTE_HANDLER;
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
#endif

	SegfaultHandler::SegfaultHandler()
	{
		auto handlerFunction = &SegfaultHandler::callback;

#ifdef __LINUX__
		handler.sa_handler = handlerFunction;

		sigemptyset(&handler.sa_mask);
		
		handler.sa_flags = 0;
		
		sigaction(SIGSEGV, &handler, nullptr);
#else
		AddVectoredExceptionHandler(1, handlerFunction);
#endif
	}
}

#endif
