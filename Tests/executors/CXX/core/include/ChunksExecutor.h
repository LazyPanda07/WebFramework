#pragma once

#include <Executors/HeavyOperationStatelessExecutor.hpp>

class ChunksExecutor : public framework::HeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doPost(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
