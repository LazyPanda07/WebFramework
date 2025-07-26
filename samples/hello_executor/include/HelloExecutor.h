#pragma once

#include <APIExecutors/BaseStatelessExecutor.hpp>

namespace executors
{
	class HelloExecutor : public framework::BaseStatelessExecutor
	{
	public:
		HelloExecutor() = default;

		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~HelloExecutor() = default;
	};
}
