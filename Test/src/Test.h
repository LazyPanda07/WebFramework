#pragma once

#include "Executors/BaseStatelessExecutor.h"

class Test : public framework::BaseStatelessExecutor
{
public:
	void init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings) override;

	void doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response) override;
};

EXECUTOR_CREATION_FUNCTION(Test)
