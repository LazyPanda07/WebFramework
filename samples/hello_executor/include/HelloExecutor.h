#pragma once

#include <Executors/StatelessExecutor.hpp>

namespace executors
{
	class HelloExecutor : public framework::StatelessExecutor
	{
	public:
		HelloExecutor() = default;

		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~HelloExecutor() = default;
	};
}
