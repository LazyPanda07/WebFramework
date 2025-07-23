#pragma once

#include "Executors/BaseStatelessExecutor.h"

class UserAgentExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;
};
