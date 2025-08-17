#pragma once

#include <APIExecutors/BaseStatelessExecutor.hpp>

class Executor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
