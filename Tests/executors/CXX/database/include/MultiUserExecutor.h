#pragma once

#include <Executors/HeavyOperationStatefulExecutor.hpp>

class MultiUserExecutor : public framework::HeavyOperationStatefulExecutor
{
private:
	std::string userId;

public:
	MultiUserExecutor();

	void init(const framework::utility::ExecutorSettings& settings) override;

	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPut(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
