#pragma once

#include <Executors/StatefulExecutor.hpp>

#include <fstream>

class UploadOctetStreamExecutor : public framework::StatefulExecutor
{
private:
	std::ofstream stream;

public:
	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
