#pragma once

#include <Executors/BaseHeavyOperationStatelessExecutor.hpp>

class ChunksExecutor : public framework::BaseHeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
