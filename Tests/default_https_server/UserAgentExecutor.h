#pragma once

#include <APIExecutors/BaseStatelessExecutor.hpp>

class UserAgentExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
