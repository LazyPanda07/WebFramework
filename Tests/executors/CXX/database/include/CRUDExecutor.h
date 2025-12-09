#pragma once

#include <Executors/StatelessExecutor.hpp>

class CRUDExecutor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
