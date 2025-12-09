#pragma once

#include <Executors/StatelessExecutor.hpp>

class UserAgentExecutor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
