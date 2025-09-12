#pragma once

#include <Executors/BaseStatelessExecutor.hpp>

class RoutePatternExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
