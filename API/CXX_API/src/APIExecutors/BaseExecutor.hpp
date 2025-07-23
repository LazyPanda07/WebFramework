#pragma once

#include <format>

#include "../HTTPRequest.hpp"
#include "../HTTPResponse.hpp"
#include "../DLLHandler.hpp"
#include "../Exceptions/NotImplementedDoMethodException.hpp"

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
	private:
		static void isImplemented
		(
			std::vector<std::string>& result,
			HTTPRequest& request,
			HTTPResponse& response,
			const std::string& methodName,
			void(BaseExecutor::* method)(HTTPRequest&, HTTPResponse&),
			BaseExecutor& executor
		);

	public:
		// virtual void init(const utility::JSONSettingsParser::ExecutorSettings& settings);

		virtual void doPost(HTTPRequest& request, HTTPResponse& response)
		{
			request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
		}

		virtual void doGet(HTTPRequest& request, HTTPResponse& response)
		{
			request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
		}

		virtual void doHead(HTTPRequest& request, HTTPResponse& response)
		{
			request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
		}

		virtual void doPut(HTTPRequest& request, HTTPResponse& response)
		{
			request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
		}

		virtual void doDelete(HTTPRequest& request, HTTPResponse& response)
		{
			request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
		}

		virtual void doPatch(HTTPRequest& request, HTTPResponse& response)
		{
			request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
		}

		virtual void doOptions(HTTPRequest& request, HTTPResponse& response)
		{
#ifdef NDEBUG
			request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
#endif

			std::vector<std::string> methods = { "OPTIONS" };
			std::string allowHeader;

			BaseExecutor::isImplemented(methods, request, response, "GET", &BaseExecutor::doGet, *this);
			BaseExecutor::isImplemented(methods, request, response, "POST", &BaseExecutor::doPost, *this);
			BaseExecutor::isImplemented(methods, request, response, "HEAD", &BaseExecutor::doHead, *this);
			BaseExecutor::isImplemented(methods, request, response, "PUT", &BaseExecutor::doPut, *this);
			BaseExecutor::isImplemented(methods, request, response, "DELETE", &BaseExecutor::doDelete, *this);
			BaseExecutor::isImplemented(methods, request, response, "PATCH", &BaseExecutor::doPatch, *this);
			BaseExecutor::isImplemented(methods, request, response, "TRACE", &BaseExecutor::doTrace, *this);
			BaseExecutor::isImplemented(methods, request, response, "CONNECT", &BaseExecutor::doConnect, *this);

			for (size_t i = 0; i < methods.size(); i++)
			{
				allowHeader += methods[i];

				if (i + 1 != methods.size())
				{
					allowHeader += ", ";
				}
			}

			response.addHeader("Allow", allowHeader);
		}

		virtual void doTrace(HTTPRequest& request, HTTPResponse& response)
		{
#ifdef NDEBUG
			request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
#endif

			response.setBody(request.getRawRequest());
		}

		virtual void doConnect(HTTPRequest& request, HTTPResponse& response)
		{
			request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
		}

		virtual utility::ExecutorType getType() const = 0;

		virtual void destroy() = 0;

		virtual ~BaseExecutor() = default;
	};
}

namespace framework
{
	inline void BaseExecutor::isImplemented
	(
		std::vector<std::string>& result,
		HTTPRequest& request,
		HTTPResponse& response,
		const std::string& methodName,
		void(BaseExecutor::* method)(HTTPRequest&, HTTPResponse&),
		BaseExecutor& executor
	)
	{
		response.setDefault();

		try
		{
			(executor.*method)(request, response);

			result.push_back(methodName);
		}
		catch (const std::exception& e)
		{
			if (!HTTPRequest::isException<exceptions::NotImplementedDoMethodException>(e))
			{
				result.push_back(methodName);
			}
		}
		catch (...)
		{
			result.push_back(methodName);
		}
	}
}

#ifdef __LINUX__
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API extern "C" __attribute__((visibility("default"))) __attribute__((used))
#else
#define WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API extern "C" __declspec(dllexport)
#endif

/**
* Macro for each BaseExecutor subclass
* Used for loading function that creates BaseExecutor subclass
*/
#define DECLARE_EXECUTOR(subclassName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void* create##subclassName##Instance()	\
{	\
	return new subclassName();	\
}

#pragma region ExportFunctions
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
