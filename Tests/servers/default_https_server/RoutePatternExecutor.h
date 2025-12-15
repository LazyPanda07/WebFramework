#pragma once

#include <Executors/StatelessExecutor.hpp>

class RoutePatternExecutor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
