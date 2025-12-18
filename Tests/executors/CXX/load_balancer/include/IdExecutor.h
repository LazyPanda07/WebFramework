#pragma once

#include <Executors/HeavyOperationStatelessExecutor.hpp>

class IdExecutor : public framework::HeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
