#pragma once

#include <Executors/StatelessExecutor.hpp>

namespace framework
{
	class PyStatelessExecutor : public StatelessExecutor
	{
	public:
		PyStatelessExecutor() = default;

		void init(const utility::ExecutorSettings& settings) override;

		void doPost(HttpRequest& request, HttpResponse& response) override;

		void doGet(HttpRequest& request, HttpResponse& response) override;

		void doHead(HttpRequest& request, HttpResponse& response) override;

		void doPut(HttpRequest& request, HttpResponse& response) override;

		void doDelete(HttpRequest& request, HttpResponse& response) override;

		void doPatch(HttpRequest& request, HttpResponse& response) override;

		void doOptions(HttpRequest& request, HttpResponse& response) override;

		void doTrace(HttpRequest& request, HttpResponse& response) override;

		void doConnect(HttpRequest& request, HttpResponse& response) override;

		~PyStatelessExecutor() = default;
	};
}
