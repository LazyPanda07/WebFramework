#pragma once

#include <APIExecutors/BaseStatelessExecutor.hpp>

class HelloExecutor : public framework::BaseStatelessExecutor
{
private:
	int64_t value;

public:
	void init(const framework::utility::ExecutorSettings& executorSettings) override;

	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doHead(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doConnect(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
