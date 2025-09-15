#pragma once

#include <Executors/BaseStatelessExecutor.hpp>

class AssetsExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
