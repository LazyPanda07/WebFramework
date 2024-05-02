#pragma once

#include <Executors/BaseStatefulExecutor.h>

class CRUDExecutor : public framework::BaseStatefulExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void destroy() override;
};
