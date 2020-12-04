#pragma once

#include "HTTPParser.h"
#include "Exceptions/NotImplementedException.h"

namespace framework
{
	class BaseExecutor
	{
	public:
		BaseExecutor() = default;

		virtual void init() = 0;

		virtual void doPost(web::HTTPParser&& request, std::string& response);

		virtual void doGet(web::HTTPParser&& request, std::string& response);

		virtual void destroy() = 0;

		virtual ~BaseExecutor() = default;
	};

	using createBaseExecutorSubclassFunction = BaseExecutor* (*)();
}

#define CREATE_BASE_EXECUTOR_SUBCLASS_INSTANCE(subclassName) extern "C" __declspec(dllexport) framework::BaseExecutor* create##subclassName##Instance()	\
																							  {	\
																								return new subclassName();	\
																							  }
