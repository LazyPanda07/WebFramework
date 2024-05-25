#pragma once

#include "Executors/BaseStatelessExecutor.h"

class IdExecutor : public framework::BaseStatel{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
