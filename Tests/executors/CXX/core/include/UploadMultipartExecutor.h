#pragma once

#include <Executors/HeavyOperationStatelessExecutor.hpp>

class UploadMultipartExecutor : public framework::HeavyOperationStatelessExecutor
{
public:
	void doPost(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
