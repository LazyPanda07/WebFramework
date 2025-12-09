#pragma once

#include <Executors/HeavyOperationStatefulExecutor.hpp>

class MultiUserExecutor : public framework::HeavyOperationStatefulExecutor
{
private:
	std::string userId;

public:
	MultiUserExecutor();

	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
