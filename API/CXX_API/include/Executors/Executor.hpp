#pragma once

#include <format>

#include "../HttpRequest.hpp"
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

			void registerDynamicFunction(std::string_view functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result)) const;

			void registerDynamicFunctionClass(std::string_view functionName, std::string_view apiType, void* functionClass) const;

			void unregisterDynamicFunction(std::string_view functionName) const;

			/**
			 * @brief Get Json structed values from initParameters section from settings file
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

			template<DatabaseImplementation T = DefaultDatabase>
			Database getOrCreateDatabase(std::string_view databaseName) const;

			template<DatabaseImplementation T = DefaultDatabase>
			Database getDatabase(std::string_view databaseName) const;

			template<DatabaseImplementation T = DefaultDatabase>
			Table getOrCreateTable(std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery) const;

			template<DatabaseImplementation T = DefaultDatabase>
			Table getTable(std::string_view databaseName, std::string_view tableName) const;

			~ExecutorSettings() = default;
		};
	}

	class Executor
	{
	private:
		static void isImplemented
		(
			std::vector<std::string>& result,
			HttpRequest& request,
			HttpResponse& response,
			const std::string& methodName,
			void(Executor::* method)(HttpRequest&, HttpResponse&),
			Executor& executor
		);

	public:
		Executor() = default;

		virtual void init(const utility::ExecutorSettings& settings);

		virtual void doPost(HttpRequest& request, HttpResponse& response);

		virtual void doGet(HttpRequest& request, HttpResponse& response);

		virtual void doHead(HttpRequest& request, HttpResponse& response);

		virtual void doPut(HttpRequest& request, HttpResponse& response);

		virtual void doDelete(HttpRequest& request, HttpResponse& response);

		virtual void doPatch(HttpRequest& request, HttpResponse& response);

		virtual void doOptions(HttpRequest& request, HttpResponse& response);

		virtual void doTrace(HttpRequest& request, HttpResponse& response);

		virtual void doConnect(HttpRequest& request, HttpResponse& response);

		virtual utility::ExecutorType getType() const = 0;

		virtual void destroy() = 0;

		virtual ~Executor() = default;
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

		inline void ExecutorSettings::registerDynamicFunction(std::string_view functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result)) const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(registerDynamicFunctionExecutorSettings, void, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result), void** exception);
			void* exception = nullptr;

			DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(registerDynamicFunctionExecutorSettings, functionName.data(), function, deleter, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}
		}

		inline void ExecutorSettings::registerDynamicFunctionClass(std::string_view functionName, std::string_view apiType, void* functionClass) const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(registerDynamicFunctionClassExecutorSettingss, void, const char* functionName, const char* apiType, void* functionClass, void** exception);
			void* exception = nullptr;

			DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(registerDynamicFunctionClassExecutorSettingss, functionName.data(), apiType.data(), functionClass, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}
		}

		inline void ExecutorSettings::unregisterDynamicFunction(std::string_view functionName) const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(unregisterDynamicFunction, void, const char* functionName, void** exception);
			void* exception = nullptr;

			DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(unregisterDynamicFunction, functionName.data(), &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}
		}

		inline JsonParser ExecutorSettings::getInitParameters() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getExecutorInitParameters, void*, void** exception);
			void* exception = nullptr;
			DllHandler& handler = DllHandler::getInstance();

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
			DllHandler& handler = DllHandler::getInstance();

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
			DllHandler& handler = DllHandler::getInstance();

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
			DllHandler& handler = DllHandler::getInstance();

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

			int result = DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorLoadType, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return static_cast<LoadType>(result);
		}

		template<DatabaseImplementation T>
		inline Database ExecutorSettings::getOrCreateDatabase(std::string_view databaseName) const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getOrCreateDatabaseExecutorSettings, void*, const char* databaseName, const char* implementationName, void** exception);
			void* exception = nullptr;
			
			Database result(static_cast<interfaces::IDatabase*>(DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateDatabaseExecutorSettings, databaseName.data(), T::databaseImplementationName.data(), &exception)));

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return result;
		}

		template<DatabaseImplementation T>
		inline Database ExecutorSettings::getDatabase(std::string_view databaseName) const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getDatabaseExecutorSettings, void*, const char* databaseName, const char* implementationName, void** exception);
			void* exception = nullptr;

			Database result(static_cast<interfaces::IDatabase*>(DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseExecutorSettings, databaseName.data(), T::databaseImplementationName.data(), &exception)));

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return result;
		}

		template<DatabaseImplementation T>
		inline Table ExecutorSettings::getOrCreateTable(std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery) const
		{
			return this->getOrCreateDatabase<T>(databaseName).getOrCreateTable(tableName, createTableQuery);
		}

		template<DatabaseImplementation T>
		inline Table ExecutorSettings::getTable(std::string_view databaseName, std::string_view tableName) const
		{
			return this->getDatabase<T>(databaseName).getTable(tableName);
		}
	}

	inline void Executor::init(const utility::ExecutorSettings& settings)
	{

	}

	inline void Executor::doPost(HttpRequest& request, HttpResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void Executor::doGet(HttpRequest& request, HttpResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void Executor::doHead(HttpRequest& request, HttpResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void Executor::doPut(HttpRequest& request, HttpResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void Executor::doDelete(HttpRequest& request, HttpResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void Executor::doPatch(HttpRequest& request, HttpResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void Executor::doOptions(HttpRequest& request, HttpResponse& response)
	{
#ifdef NDEBUG
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
#endif

		std::vector<std::string> methods = { "OPTIONS" };
		std::string allowHeaders;

		Executor::isImplemented(methods, request, response, "GET", &Executor::doGet, *this);
		Executor::isImplemented(methods, request, response, "POST", &Executor::doPost, *this);
		Executor::isImplemented(methods, request, response, "HEAD", &Executor::doHead, *this);
		Executor::isImplemented(methods, request, response, "PUT", &Executor::doPut, *this);
		Executor::isImplemented(methods, request, response, "DELETE", &Executor::doDelete, *this);
		Executor::isImplemented(methods, request, response, "PATCH", &Executor::doPatch, *this);
		Executor::isImplemented(methods, request, response, "TRACE", &Executor::doTrace, *this);
		Executor::isImplemented(methods, request, response, "CONNECT", &Executor::doConnect, *this);

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

	inline void Executor::doTrace(HttpRequest& request, HttpResponse& response)
	{
#ifdef NDEBUG
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
#endif

		response.setBody(request.getRawRequest());
	}

	inline void Executor::doConnect(HttpRequest& request, HttpResponse& response)
	{
		request.throwException<exceptions::NotImplementedDoMethodException>(__func__, typeid(*this).name());
	}

	inline void Executor::isImplemented
	(
		std::vector<std::string>& result,
		HttpRequest& request,
		HttpResponse& response,
		const std::string& methodName,
		void(Executor::* method)(HttpRequest&, HttpResponse&),
		Executor& executor
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
			if (!HttpRequest::isException<exceptions::NotImplementedDoMethodException>(e))
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
* Macro for each Executor subclass
* Used for loading function that creates Executor subclass
*/
#define DEFINE_EXECUTOR(subclassName) WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API void* create##subclassName##CXXInstance()	\
{	\
	return new subclassName();	\
}

#pragma region ExportFunctions
WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXExecutorInit(void* implementation, void* executorSettings)
{
	static_cast<framework::Executor*>(implementation)->init(framework::utility::ExecutorSettings(executorSettings));
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoPost(void* implementation, framework::interfaces::IHttpRequest* request, framework::interfaces::IHttpResponse* response)
{
	framework::HttpRequest requestWrapper(request);
	framework::HttpResponse responseWrapper(response);

	static_cast<framework::Executor*>(implementation)->doPost(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoGet(void* implementation, framework::interfaces::IHttpRequest* request, framework::interfaces::IHttpResponse* response)
{
	framework::HttpRequest requestWrapper(request);
	framework::HttpResponse responseWrapper(response);

	static_cast<framework::Executor*>(implementation)->doGet(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoHead(void* implementation, framework::interfaces::IHttpRequest* request, framework::interfaces::IHttpResponse* response)
{
	framework::HttpRequest requestWrapper(request);
	framework::HttpResponse responseWrapper(response);

	static_cast<framework::Executor*>(implementation)->doHead(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoPut(void* implementation, framework::interfaces::IHttpRequest* request, framework::interfaces::IHttpResponse* response)
{
	framework::HttpRequest requestWrapper(request);
	framework::HttpResponse responseWrapper(response);

	static_cast<framework::Executor*>(implementation)->doPut(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoDelete(void* implementation, framework::interfaces::IHttpRequest* request, framework::interfaces::IHttpResponse* response)
{
	framework::HttpRequest requestWrapper(request);
	framework::HttpResponse responseWrapper(response);

	static_cast<framework::Executor*>(implementation)->doDelete(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoPatch(void* implementation, framework::interfaces::IHttpRequest* request, framework::interfaces::IHttpResponse* response)
{
	framework::HttpRequest requestWrapper(request);
	framework::HttpResponse responseWrapper(response);

	static_cast<framework::Executor*>(implementation)->doPatch(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoOptions(void* implementation, framework::interfaces::IHttpRequest* request, framework::interfaces::IHttpResponse* response)
{
	framework::HttpRequest requestWrapper(request);
	framework::HttpResponse responseWrapper(response);

	static_cast<framework::Executor*>(implementation)->doOptions(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoTrace(void* implementation, framework::interfaces::IHttpRequest* request, framework::interfaces::IHttpResponse* response)
{
	framework::HttpRequest requestWrapper(request);
	framework::HttpResponse responseWrapper(response);

	static_cast<framework::Executor*>(implementation)->doTrace(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDoConnect(void* implementation, framework::interfaces::IHttpRequest* request, framework::interfaces::IHttpResponse* response)
{
	framework::HttpRequest requestWrapper(request);
	framework::HttpResponse responseWrapper(response);

	static_cast<framework::Executor*>(implementation)->doConnect(requestWrapper, responseWrapper);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline framework::utility::ExecutorType webFrameworkCXXGetType(void* implementation)
{
	return static_cast<framework::Executor*>(implementation)->getType();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDestroyExecutor(void* implementation)
{
	static_cast<framework::Executor*>(implementation)->destroy();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkCXXDeleteExecutor(void* implementation)
{
	delete static_cast<framework::Executor*>(implementation);
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void initializeWebFrameworkCXX(const char* webFrameworkSharedLibraryPath)
{
	framework::utility::initializeWebFramework(webFrameworkSharedLibraryPath);
}
#pragma endregion
