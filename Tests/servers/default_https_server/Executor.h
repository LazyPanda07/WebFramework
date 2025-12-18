#pragma once

#include <Executors/StatelessExecutor.hpp>

class Executor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
