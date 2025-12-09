#pragma once

#include <Executors/HeavyOperationStatelessExecutor.hpp>

class IdExecutor : public framework::HeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
