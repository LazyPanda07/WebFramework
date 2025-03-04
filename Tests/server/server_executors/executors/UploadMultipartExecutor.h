#pragma once

#include "Executors/BaseHeavyOperationStatefulExecutor.h"

class UploadMultipartExecutor : public framework::BaseHeavyOperationStatefulExecutor
{
public:
	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
