#pragma once

#include <Executors/StatelessExecutor.hpp>

class AssetsExecutor : public framework::StatelessExecutor
{
public:
	void init(const framework::utility::ExecutorSettings& settings) override;

	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

	void doDelete(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
