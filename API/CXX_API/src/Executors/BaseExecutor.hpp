#pragma once

#include "../HTTPRequest.hpp"
#include "../HTTPResponse.hpp"
#include "../DLLHandler.hpp"

namespace framework
{
	namespace utility
	{
		enum class ExecutorType
		{
			none,
			stateful,
			stateless,
			heavyOperationStateful,
			heavyOperationStateless
		};
	}

	class BaseExecutor
	{
	public:
		// virtual void init(const utility::JSONSettingsParser::ExecutorSettings& settings);

		virtual void doPost(HTTPRequest& request, HTTPResponse& response)
		{

		}

		virtual void doGet(HTTPRequest& request, HTTPResponse& response)
		{

		}

		virtual void doHead(HTTPRequest& request, HTTPResponse& response)
		{

		}

		virtual void doPut(HTTPRequest& request, HTTPResponse& response)
		{

		}

		virtual void doDelete(HTTPRequest& request, HTTPResponse& response)
		{

		}

		virtual void doPatch(HTTPRequest& request, HTTPResponse& response)
		{

		}

		virtual void doOptions(HTTPRequest& request, HTTPResponse& response)
		{

		}

		virtual void doTrace(HTTPRequest& request, HTTPResponse& response)
		{

		}

		virtual void doConnect(HTTPRequest& request, HTTPResponse& response)
		{

		}

		virtual utility::ExecutorType getType() const = 0;

		virtual void destroy() = 0;

		virtual ~BaseExecutor() = default;
	};
}

#ifdef __LINUX__
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API extern "C" __attribute__((visibility("default")))
#else
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API extern "C" __declspec(dllexport)
#endif

/**
* Macro for each BaseExecutor subclass
* Used for loading function that creates BaseExecutor subclass
*/
#define DECLARE_EXECUTOR(subclassName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API framework::BaseExecutor* create##subclassName##Instance()	\
{	\
	return new subclassName();	\
}

#pragma region ExportedFunctions
WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoPost(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doPost(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoGet(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doGet(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoHead(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doHead(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoPut(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doPut(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoDelete(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doDelete(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoPatch(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doPatch(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoOptions(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doOptions(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoTrace(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doTrace(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoConnect(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doConnect(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline framework::utility::ExecutorType webFrameworkGetType(void* implementation)
{
	return static_cast<framework::BaseExecutor*>(implementation)->getType();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDestroyExecutor(void* implementation)
{
	static_cast<framework::BaseExecutor*>(implementation)->destroy();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void initializeWebFrameworkForExecutors(const char* webFrameworkSharedLibraryPath)
{
	framework::utility::initializeWebFramework(webFrameworkSharedLibraryPath);
}
#pragma endregion
