#pragma once

#include <Executors/StatelessExecutor.hpp>

class RoutePatternExecutor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
