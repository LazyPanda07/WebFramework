#include "C_API/WebFrameworkWebSocketExecutorsCApi.h"

#include <Log.h>

#include "Exceptions/AlreadyLoggedException.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {} in {} function", "C_API", e.what(), __func__); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error(std::format("Something went wrong in file: {} on line: {}", __FILE__, __LINE__));

char* getFramePayload(FrameObject frame, uint64_t* size, Exception* exception)
{
	try
	{
		return nullptr;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

int getFrameType(FrameObject frame, Exception* exception)
{
	try
	{
		return 0;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}
