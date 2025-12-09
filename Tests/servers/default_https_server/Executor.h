#pragma once

#include <Executors/StatelessExecutor.hpp>

class Executor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
