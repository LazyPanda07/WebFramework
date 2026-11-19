#pragma once

#include <Executors/StatelessExecutor.hpp>

class MultiplePorts : public framework::StatelessExecutor
{
public:
	MultiplePorts() = default;

	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

	~MultiplePorts() = default;
};
