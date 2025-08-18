#pragma once

#include <APIExecutors/BaseHeavyOperationStatelessExecutor.hpp>

class DownloadExecutor : public framework::BaseHeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
