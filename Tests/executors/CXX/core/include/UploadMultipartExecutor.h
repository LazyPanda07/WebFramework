#pragma once

#include <Executors/BaseHeavyOperationStatelessExecutor.hpp>

class UploadMultipartExecutor : public framework::BaseHeavyOperationStatelessExecutor
{
public:
	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
