#pragma once

#include <Executors/HeavyOperationStatefulExecutor.hpp>

namespace framework
{
	class PyHeavyOperationStatefulExecutor : public HeavyOperationStatefulExecutor
	{
	public:
		PyHeavyOperationStatefulExecutor() = default;

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

		void destroy() override;

		~PyHeavyOperationStatefulExecutor() = default;
	};
}

