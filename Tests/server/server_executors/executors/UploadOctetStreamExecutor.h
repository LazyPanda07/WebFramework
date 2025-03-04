#pragma once

#include "Executors/BaseStatefulExecutor.h"

#include <fstream>

class UploadOctetStreamExecutor : public framework::BaseStatefulExecutor
{
private:
	std::ofstream stream;

public:
	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
