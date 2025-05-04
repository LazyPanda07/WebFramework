#pragma once

#include <Executors/BaseStatelessExecutor.h>

class HelloExecutor : public framework::BaseStatelessExecutor
{
public:
	HelloExecutor() = default;

	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	~HelloExecutor() = default;
};
