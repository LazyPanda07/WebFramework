#pragma once

#include "Executors/BaseHeavyOperationStatelessExecutor.h"

class ChunksExecutor : public framework::BaseHeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
