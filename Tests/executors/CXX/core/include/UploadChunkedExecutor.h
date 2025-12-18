#pragma once

#include <Executors/HeavyOperationStatelessExecutor.hpp>

class UploadChunkedExecutor : public framework::HeavyOperationStatelessExecutor
{
public:
	void doPost(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
