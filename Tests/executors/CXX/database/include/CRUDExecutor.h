#pragma once

#include <Executors/StatelessExecutor.hpp>

class CRUDExecutor : public framework::StatelessExecutor
{
public:
	void init(const framework::utility::ExecutorSettings& settings) override;

	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPut(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPatch(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doDelete(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
