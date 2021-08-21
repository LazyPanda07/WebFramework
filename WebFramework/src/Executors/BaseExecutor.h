/*
* Executors is classes that can process specific request from clients
* For example following link send request to "/test" path http://127.0.0.1/test
* You can create and configure your class that would be processing this path
* Base class for all executors
*/
#pragma once

#include "headers.h"

#include "WebNetwork/HTTPRequest.h"
#include "WebNetwork/HTTPResponse.h"
#include "Exceptions/NotImplementedException.h"
#include "Utility/JSONSettingsParser.h"

namespace framework
{
	/// <summary>
	/// Base class for all executors
	/// </summary>
	class WEB_FRAMEWORK_API BaseExecutor
	{
	public:
		enum class executorType
		{
			none,
			stateful,
			stateless,
			heavyOperationStateful,
			heavyOperationStateless
		};

	public:
		BaseExecutor() = default;

		/// <summary>
		/// Initializing executor before use
		/// </summary>
		/// <param name="settings">providing parameters from .json to executor</param>
		virtual void init(const utility::JSONSettingsParser::ExecutorSettings& settings);

		/// <summary>
		/// Process POST request
		/// </summary>
		/// <param name="request">special class for requests</param>
		/// <param name="response">special class for responses</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doPost(HTTPRequest&& request, HTTPResponse& response);

		/// <summary>
		/// Process GET request
		/// </summary>
		/// <param name="request">special class for requests</param>
		/// <param name="response">special class for responses</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doGet(HTTPRequest&& request, HTTPResponse& response);

		/// <summary>
		/// Process HEAD request
		/// </summary>
		/// <param name="request">special class for requests</param>
		/// <param name="response">special class for responses</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doHead(HTTPRequest&& request, HTTPResponse& response);

		/// <summary>
		/// Process PUT request
		/// </summary>
		/// <param name="request">special class for requests</param>
		/// <param name="response">special class for responses</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doPut(HTTPRequest&& request, HTTPResponse& response);

		/// <summary>
		/// Process DELETE request
		/// </summary>
		/// <param name="request">special class for requests</param>
		/// <param name="response">special class for responses</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doDelete(HTTPRequest&& request, HTTPResponse& response);

		/// <summary>
		/// Process OPTIONS request
		/// </summary>
		/// <param name="request">special class for requests</param>
		/// <param name="response">special class for responses</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doOptions(HTTPRequest&& request, HTTPResponse& response);

		/// <summary>
		/// Process TRACE request
		/// </summary>
		/// <param name="request">special class for requests</param>
		/// <param name="response">special class for responses</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		virtual void doTrace(HTTPRequest&& request, HTTPResponse& response);

		/// <summary>
		/// <para>Destroy and unload executor</para>
		/// <para>Executors inherited from BaseStatelessExecutor no need this method</para>
		/// </summary>
		virtual void destroy() = 0;

		/// <summary>
		/// <para>By default all executors must be inherited from BaseStatelessExecutor or BaseStatefullExecutor</para>
		/// <para>BaseStatelessExecutor override this method by getting executorType::stateless</para>
		/// <para>BaseStatefulExecutor override this method by getting executorType::stateful</para>
		/// <para>ExecutorsManager can manage executors by getting type of executorType enum class</para>
		/// </summary>
		/// <returns>stateful, stateless or none value</returns>
		virtual executorType getType() const = 0;

		virtual ~BaseExecutor() = default;
	};

	/// <summary>
	/// Shortcut to creating BaseExecutor subclass function signature
	/// </summary>
	using createBaseExecutorSubclassFunction = BaseExecutor* (*)();
}

/// <summary>
/// <para>Macro for each BaseExecutor subclass</para>
/// <para>Used for loading function that creates BaseExecutor subclass</para>
/// </summary>
#define CREATE_BASE_EXECUTOR_SUBCLASS_INSTANCE(subclassName) extern "C" __declspec(dllexport) framework::BaseExecutor* create##subclassName##Instance()	\
{	\
	return new subclassName();	\
}
