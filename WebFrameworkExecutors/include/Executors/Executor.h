/**
* Executors is classes that can process specific request from clients
* For example following link send request to "/test" path http://127.0.0.1/test
* You can create and configure your class that would be processing this path
* Base class for all executors
*/
#pragma once

#include <functional>

#include "WebInterfaces/IHttpRequest.h"
#include "Exceptions/NotImplementedException.h"
#include "Utility/JSONSettingsParser.h"
#include "ExecutorsConstants.h"

namespace framework
{
	/// <summary>
	/// Base class for all executors
	/// </summary>
	class EXECUTORS_API Executor
	{
	public:
		static void (Executor::* getMethod(std::string_view methodName))(interfaces::IHttpRequest&, interfaces::IHttpResponse&);

	public:
		Executor() = default;

		/// <summary>
		/// Initializing executor before use
		/// </summary>
		/// <param name="settings">providing parameters from .json to executor</param>
		virtual void init(const utility::JSONSettingsParser::ExecutorSettings& settings);

		/// <summary>
		/// Process POST request
		/// </summary>
		/// <param name="request">Request controller</param>
		/// <param name="response">Response controller</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doPost(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

		/// <summary>
		/// Process GET request
		/// </summary>
		/// <param name="request">Request controller</param>
		/// <param name="response">Response controller</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doGet(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

		/// <summary>
		/// Process HEAD request
		/// </summary>
		/// <param name="request">Request controller</param>
		/// <param name="response">Response controller</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doHead(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

		/// <summary>
		/// Process PUT request
		/// </summary>
		/// <param name="request">Request controller</param>
		/// <param name="response">Response controller</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doPut(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

		/// <summary>
		/// Process DELETE request
		/// </summary>
		/// <param name="request">Request controller</param>
		/// <param name="response">Response controller</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doDelete(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

		/**
		 * @brief Process PATCH request
		 * @param request Request controller
		 * @param response Response controller
		 */
		virtual void doPatch(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

		/// <summary>
		/// Process OPTIONS request
		/// </summary>
		/// <param name="request">Request controller</param>
		/// <param name="response">Response controller</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doOptions(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

		/// <summary>
		/// Process TRACE request
		/// </summary>
		/// <param name="request">Request controller</param>
		/// <param name="response">Response controller</param>
		virtual void doTrace(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

		/**
		 * @brief Process CONNECT request
		 * @param request Request controller
		 * @param response Response controller
		 */
		virtual void doConnect(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

		/// <summary>
		/// <para>By default all executors must be inherited from StatelessExecutor or BaseStatefullExecutor</para>
		/// <para>StatelessExecutor override this method by getting ExecutorType::stateless</para>
		/// <para>StatefulExecutor override this method by getting ExecutorType::stateful</para>
		/// <para>ExecutorsManager can manage executors by getting type of ExecutorType enum class</para>
		/// </summary>
		/// <returns>stateful, stateless or none value</returns>
		virtual utility::ExecutorType getType() const = 0;

		/// <summary>
		/// <para>Destroy and unload executor</para>
		/// <para>Executors inherited from StatelessExecutor no need this method</para>
		/// </summary>
		virtual void destroy() = 0;

		virtual ~Executor() = default;
	};

	using CreateExecutorSignature = void* (*)();
	using CreateExecutorFunction = std::function<void* ()>;
	using InitializeWebFrameworkInExecutor = void(*)(const char*);
}

#ifdef __LINUX__
#define EXPORT_EXECUTOR_FUNCTION extern "C" __attribute__((visibility("default")))
#else
#define EXPORT_EXECUTOR_FUNCTION extern "C" __declspec(dllexport)
#endif

/**
* Macro for each Executor subclass
* Used for loading function that creates Executor subclass
*/
#define DEFINE_EXECUTOR(subclassName) EXPORT_EXECUTOR_FUNCTION void* create##subclassName##Instance()	\
{	\
	return new subclassName();	\
}
