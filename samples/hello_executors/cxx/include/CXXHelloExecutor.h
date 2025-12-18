#pragma once

#include <Executors/StatelessExecutor.hpp>

namespace executors
{
	class CXXHelloExecutor : public framework::StatelessExecutor
	{
	public:
		CXXHelloExecutor() = default;

		void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

		~CXXHelloExecutor() = default;
	};
}
