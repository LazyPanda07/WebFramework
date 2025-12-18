#pragma once

#include <Executors/StatelessExecutor.hpp>

class AssetsExecutor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPost(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doDelete(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
