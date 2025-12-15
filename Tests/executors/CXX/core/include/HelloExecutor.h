#pragma once

#include <Executors/StatelessExecutor.hpp>

class HelloExecutor : public framework::StatelessExecutor
{
private:
	int64_t value;

public:
	void init(const framework::utility::ExecutorSettings& executorSettings) override;

	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPost(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doHead(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPut(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doDelete(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPatch(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doConnect(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
