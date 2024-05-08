#pragma once

#include <Executors/BaseStatefulExecutor.h>

class MultiUserExecutor : public framework::BaseStatefulExecutor
{
private:
	size_t userId;

public:
	MultiUserExecutor();

	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
