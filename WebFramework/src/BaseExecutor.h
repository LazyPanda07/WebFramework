#pragma once

#include "HTTPParser.h"
#include "ExecutorException.h"

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
}
