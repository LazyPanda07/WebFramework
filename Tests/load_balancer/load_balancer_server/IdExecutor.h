#pragma once

#include "Executors/BaseStatelessExecutor.h"

class IdExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
