#pragma once

#include <Executors/HeavyOperationStatelessExecutor.hpp>

class UploadChunkedExecutor : public framework::HeavyOperationStatelessExecutor
{
public:
	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
