#pragma once

#include "Executors/BaseStatelessExecutor.h"

class Index : public framework::BaseStatelessExecutor
{
public:
	void init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings) override;

	void doGet(framework::HTTPRequest&& request, framework::HTTPResponse& response) override;
};

CREATE_BASE_EXECUTOR_SUBCLASS_INSTANCE(Index)
