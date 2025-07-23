#pragma once

#include "Executors/BaseHeavyOperationStatelessExecutor.h"

class UploadMultipartExecutor : public framework::BaseHeavyOperationStatelessExecutor
{
public:
	void doPost(framework::HTTPRequestExecutors& request, framework::HTTPResponseExecutors& response) override;
};
