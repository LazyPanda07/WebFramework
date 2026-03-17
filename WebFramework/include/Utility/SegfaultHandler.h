#pragma once

#ifdef __WITH_STACKTRACE__

#ifdef __LINUX__
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#else
#include <Windows.h>
#endif

namespace framework::utility
{
	class SegfaultHandler
	{
	private:
#ifdef __LINUX__
		struct sigaction handler;
#endif

	private:
		static void printStacktrace();

#ifdef __LINUX__
		static void callback(int signal);
#else
		static LONG WINAPI callback(EXCEPTION_POINTERS* information);
#endif

	public:
		SegfaultHandler();

		~SegfaultHandler() = default;
	};
}

#endif
