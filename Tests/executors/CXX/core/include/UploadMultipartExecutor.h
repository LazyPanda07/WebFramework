#pragma once

#include <Executors/HeavyOperationStatelessExecutor.hpp>

class UploadMultipartExecutor : public framework::HeavyOperationStatelessExecutor
{
public:
	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
