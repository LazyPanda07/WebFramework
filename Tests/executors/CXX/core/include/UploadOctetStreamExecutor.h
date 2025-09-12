#pragma once

#include <Executors/BaseStatefulExecutor.hpp>

#include <fstream>

class UploadOctetStreamExecutor : public framework::BaseStatefulExecutor
{
private:
	int64_t currentSize;
	std::ofstream stream;

public:
	void init(const framework::utility::ExecutorSettings& settings) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
