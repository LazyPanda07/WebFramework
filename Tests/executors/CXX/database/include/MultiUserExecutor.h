#pragma once

#include <Executors/HeavyOperationStatefulExecutor.hpp>

class MultiUserExecutor : public framework::HeavyOperationStatefulExecutor
{
private:
	std::string userId;

public:
	MultiUserExecutor();

	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPost(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPut(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
