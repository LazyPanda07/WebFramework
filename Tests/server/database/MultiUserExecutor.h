#pragma once

#include <Executors/BaseHeavyOperationStatefulExecutor.h>

class MultiUserExecutor : public framework::BaseHeavyOperationStatefulExecutor
{
private:
	std::string userId;

public:
	MultiUserExecutor();

	void doGet(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;

	void doPost(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;

	void doPut(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;
};
