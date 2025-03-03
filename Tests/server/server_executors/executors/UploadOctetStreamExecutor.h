#pragma once

#include "Executors/BaseHeavyOperationStatefulExecutor.h"

#include <fstream>

class UploadOctetStreamExecutor : public framework::BaseHeavyOperationStatefulExecutor
{
private:
	std::ofstream stream;

public:
	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
