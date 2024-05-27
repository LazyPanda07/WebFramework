#pragma once

#include "Executors/BaseStatelessExecutor.h"

class Executor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
