#pragma once

#include "Executors/BaseHeavyOperationStatelessExecutor.h"

class DownloadExecutor : public framework::BaseHeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
