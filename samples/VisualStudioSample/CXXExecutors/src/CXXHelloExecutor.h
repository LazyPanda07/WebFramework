#pragma once

#include <Executors/StatelessExecutor.hpp>

class CXXHelloExecutor : public framework::StatelessExecutor
{
public:
	CXXHelloExecutor() = default;

	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	~CXXHelloExecutor() = default;
};
