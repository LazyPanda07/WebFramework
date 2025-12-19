#pragma once

#include <Executors/StatelessExecutor.hpp>

namespace executors
{
	class HelloExecutor : public framework::StatelessExecutor
	{
	public:
		HelloExecutor() = default;

		void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

		~HelloExecutor() = default;
	};
}
