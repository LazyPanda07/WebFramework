#pragma once

#include "WebNetwork/HTTPRequest.h"
#include "Exceptions/NotImplementedException.h"
#include "Utility/XMLSettingsParser.h"

namespace framework
{
	class BaseExecutor
	{
	public:
		BaseExecutor() = default;

		virtual void init(const utility::XMLSettingsParser::ExecutorSettings& settings) = 0;

		virtual void doPost(HTTPRequest&& request, std::string& response);

		virtual void doGet(HTTPRequest&& request, std::string& response);

		virtual void destroy() = 0;

		virtual ~BaseExecutor() = default;
	};

	using createBaseExecutorSubclassFunction = BaseExecutor* (*)();
}

#define CREATE_BASE_EXECUTOR_SUBCLASS_INSTANCE(subclassName) extern "C" __declspec(dllexport) framework::BaseExecutor* create##subclassName##Instance()	\
																							  {	\
																								return new subclassName();	\
																							  }
