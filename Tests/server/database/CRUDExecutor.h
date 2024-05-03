#pragma once

#include <Executors/BaseStatelessExecutor.h>

class CRUDExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
