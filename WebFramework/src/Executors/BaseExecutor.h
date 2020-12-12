#pragma once

#include "WebNetwork/HTTPRequest.h"
#include "WebNetwork/HTTPResponse.h"
#include "Exceptions/NotImplementedException.h"
#include "Utility/XMLSettingsParser.h"

#ifdef EXECUTOR_FROM_DLL
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // EXECUTOR_FROM_DLL

namespace framework
{
	class WEB_FRAMEWORK_API BaseExecutor
	{
	public:
		enum class executorType
		{
			stateful,
			stateless,
			none
		};

	public:
		BaseExecutor() = default;

		virtual void init(const utility::XMLSettingsParser::ExecutorSettings& settings) = 0;

		virtual void doPost(HTTPRequest&& request, HTTPResponse& response);

		virtual void doGet(HTTPRequest&& request, HTTPResponse& response);

		virtual void doHead(HTTPRequest&& request, HTTPResponse& response);

		virtual void doPut(HTTPRequest&& request, HTTPResponse& response);

		virtual void doDelete(HTTPRequest&& request, HTTPResponse& response);

		virtual void doOptions(HTTPRequest&& request, HTTPResponse& response);

		virtual void doTrace(HTTPRequest&& request, HTTPResponse& response);

		virtual void destroy() = 0;

		virtual executorType getType() const = 0;

		virtual ~BaseExecutor() = default;
	};

	using createBaseExecutorSubclassFunction = BaseExecutor* (*)();
}

#define CREATE_BASE_EXECUTOR_SUBCLASS_INSTANCE(subclassName) extern "C" __declspec(dllexport) framework::BaseExecutor* create##subclassName##Instance()	\
{	\
	return new subclassName();	\
}
