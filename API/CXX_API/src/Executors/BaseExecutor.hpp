#pragma once

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

		virtual void doPost(HTTPRequest& request, HTTPResponse& response);

		virtual void doGet(HTTPRequest& request, HTTPResponse& response);

		virtual void doHead(HTTPRequest& request, HTTPResponse& response);

		virtual void doPut(HTTPRequest& request, HTTPResponse& response);

		virtual void doDelete(HTTPRequest& request, HTTPResponse& response);

		virtual void doPatch(HTTPRequest& request, HTTPResponse& response);

		virtual void doOptions(HTTPRequest& request, HTTPResponse& response);

		virtual void doTrace(HTTPRequest& request, HTTPResponse& response);

		virtual void doConnect(HTTPRequest& request, HTTPResponse& response);

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

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoPost(void* implementation)
{

}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoGet(void* implementation)
{

}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoHead(void* implementation)
{

}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoPut(void* implementation)
{

}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoDelete(void* implementation)
{

}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoPatch(void* implementation)
{

}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoOptions(void* implementation)
{

}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoTrace(void* implementation)
{

}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDoConnect(void* implementation)
{

}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline framework::utility::ExecutorType webFrameworkGetType(void* implementation)
{
	return static_cast<framework::BaseExecutor*>(implementation)->getType();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDestroyExecutor(void* implementation)
{
	static_cast<framework::BaseExecutor*>(implementation)->destroy();
}
