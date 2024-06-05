#pragma once

#include <Executors/BaseHeavyOperationStatefulExecutor.h>

class MultiUserExecutor : public framework::BaseHeavyOperationStatefulExecutor
{
private:
	std::string userId;

public:
	MultiUserExecutor();

	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
