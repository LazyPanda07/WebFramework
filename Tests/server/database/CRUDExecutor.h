#pragma once

#include <Executors/BaseStatelessExecutor.h>

class CRUDExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;

	void doPost(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;

	void doPut(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;

	void doPatch(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;

	void doDelete(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;
};
