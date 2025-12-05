#pragma once

#include <format>

#include "../HttpRequest.hpp"
#include "../HttpResponse.hpp"
#include "../DLLHandler.hpp"
#include "../Exceptions/NotImplementedDoMethodException.hpp"

namespace framework
{
	namespace utility
	{
		/**
		 * @brief Behavior of created Executor
		 */
		enum class ExecutorType
		{
			/**
			 * @brief Create Executor for each client
			 */
			stateful,
			/**
			 * @brief Create Executor once while server is initializing
			 */
			stateless,
			/**
			 * @brief Create Executor for each client. If server type is threadPool then DO methods runs in thread pool
			 */
			heavyOperationStateful,
			/**
			 * @brief Create Executor once while server is initializing. If server type is threadPool then DO methods runs in thread pool
			 */
			heavyOperationStateless
		};

		class ExecutorSettings
		{
		public:
			/**
			 * @brief When Executors created
			 */
			enum class LoadType
			{
				/**
				 * @brief Create at initialization
				 */
				initialization,
				/**
				 * @brief Create if client connected
				 */
				dynamic,
				/**
				 * @brief Signal that error happened
				 */
				none
			};

		private:
			void* implementation;

		public:
			ExecutorSettings(void* implementation);

			/**
			 * @brief Get JSON structed values from initParameters section from settings file
			 * @return
			 */
			JsonParser getInitParameters() const;

			/**
			 * @brief Get executor name
			 * @return
			 */
			std::string getName() const;

			/**
			 * @brief Get filter by User-Agent header
			 * @return
			 */
			std::string getUserAgentFilter() const;

			/**
			 * @brief Get API language of Executor
			 * @return
			 */
			std::string getAPIType() const;

			/**
			 * @brief Get LoadType of Executor
			 * @return
			 */
			LoadType getLoadType() const;

			~ExecutorSettings() = default;
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
		BaseExecutor() = default;

		virtual void init(const utility::ExecutorSettings& settings);

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

namespace framework
{
	namespace utility
	{
		inline ExecutorSettings::ExecutorSettings(void* implementation) :
			implementation(implementation)
		{

		}

		inline JsonParser ExecutorSettings::getInitParameters() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getExecutorInitParameters, void*, void** exception);
			void* exception = nullptr;
			DLLHandler& handler = DLLHandler::getInstance();

			void* result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorInitParameters, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return JsonParser(handler.getString(result));
		}

		inline std::string ExecutorSettings::getName() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getExecutorName, void*, void** exception);
			void* exception = nullptr;
			DLLHandler& handler = DLLHandler::getInstance();

			void* result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorName, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return handler.getString(result);
		}

		inline std::string ExecutorSettings::getUserAgentFilter() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getExecutorUserAgentFilter, void*, void** exception);
			void* exception = nullptr;
			DLLHandler& handler = DLLHandler::getInstance();

			void* result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorUserAgentFilter, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return handler.getString(result);
		}

		inline std::string ExecutorSettings::getAPIType() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getExecutorAPIType, void*, void** exception);
			void* exception = nullptr;
			DLLHandler& handler = DLLHandler::getInstance();

			void* result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorAPIType, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return handler.getString(result);
		}

		inline ExecutorSettings::LoadType ExecutorSettings::getLoadType() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getExecutorLoadType, int, void** exception);
			void* exception = nullptr;

			int result = DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorLoadType, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return static_cast<LoadType>(result);
		}
	}

	inline void BaseExecutor::init(const utility::ExecutorSettings& settings)
	{

	}

	inline void BaseExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void BaseExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void BaseExecutor::doHead(HTTPRequest& request, HTTPResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void BaseExecutor::doPut(HTTPRequest& request, HTTPResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void BaseExecutor::doDelete(HTTPRequest& request, HTTPResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void BaseExecutor::doPatch(HTTPRequest& request, HTTPResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void BaseExecutor::doOptions(HTTPRequest& request, HTTPResponse& response)
	{
#ifdef NDEBUG
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
#endif

		std::vector<std::string> methods = { "OPTIONS" };
		std::string allowHeaders;

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
			allowHeaders += methods[i];

			if (i + 1 != methods.size())
			{
				allowHeaders += ", ";
			}
		}

		response.addHeader("Allow", allowHeaders);
	}

	inline void BaseExecutor::doTrace(HTTPRequest& request, HTTPResponse& response)
	{
#ifdef NDEBUG
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
#endif

		response.setBody(request.getRawRequest());
	}

	inline void BaseExecutor::doConnect(HTTPRequest& request, HTTPResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

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
#define DEFINE_EXECUTOR(subclassName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void* create##subclassName##CXXInstance()	\
{	\
	return new subclassName();	\
}

#pragma region ExportFunctions
WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXExecutorInit(void* implementation, void* executorSettings)
{
	static_cast<framework::BaseExecutor*>(implementation)->init(framework::utility::ExecutorSettings(executorSettings));
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoPost(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doPost(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoGet(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doGet(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoHead(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doHead(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoPut(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doPut(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoDelete(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doDelete(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoPatch(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doPatch(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoOptions(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doOptions(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoTrace(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doTrace(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoConnect(void* implementation, framework::interfaces::IHTTPRequest* request, framework::interfaces::IHTTPResponse* response)
{
	framework::HTTPRequest requestWrapper(request);
	framework::HTTPResponse responseWrapper(response);

	static_cast<framework::BaseExecutor*>(implementation)->doConnect(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline framework::utility::ExecutorType webFrameworkCXXGetType(void* implementation)
{
	return static_cast<framework::BaseExecutor*>(implementation)->getType();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDestroyExecutor(void* implementation)
{
	static_cast<framework::BaseExecutor*>(implementation)->destroy();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDeleteExecutor(void* implementation)
{
	delete static_cast<framework::BaseExecutor*>(implementation);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void initializeWebFrameworkCXX(const char* webFrameworkSharedLibraryPath)
{
	framework::utility::initializeWebFramework(webFrameworkSharedLibraryPath);
}
#pragma endregion
