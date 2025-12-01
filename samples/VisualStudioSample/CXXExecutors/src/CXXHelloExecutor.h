#pragma once

#include <Executors/BaseStatelessExecutor.hpp>

class CXXHelloExecutor : public framework::BaseStatelessExecutor
{
public:
	CXXHelloExecutor() = default;

	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	~CXXHelloExecutor() = default;
};
