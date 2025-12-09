#pragma once

#include <Executors/HeavyOperationStatelessExecutor.hpp>

class ChunksExecutor : public framework::HeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
