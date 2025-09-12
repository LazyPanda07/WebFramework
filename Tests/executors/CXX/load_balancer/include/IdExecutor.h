#pragma once

#include <Executors/BaseHeavyOperationStatelessExecutor.hpp>

class IdExecutor : public framework::BaseHeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
