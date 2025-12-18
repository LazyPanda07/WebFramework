#pragma once

#include <Executors/StatefulExecutor.hpp>

#include <fstream>

class UploadOctetStreamExecutor : public framework::StatefulExecutor
{
private:
	std::ofstream stream;

public:
	void doPost(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
