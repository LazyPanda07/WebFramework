#pragma once

#include <Executors/StatelessExecutor.hpp>

class RedisExecutor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPost(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPut(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
