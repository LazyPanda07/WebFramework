#pragma once

#include "Executors/BaseStatelessExecutor.h"

class IdExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;

	void doPost(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;
};
