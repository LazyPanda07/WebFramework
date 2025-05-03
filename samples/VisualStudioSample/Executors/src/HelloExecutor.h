#pragma once

#include <Executors/BaseStatelessExecutor.h>

class HelloExecutor : public framework::BaseStatelessExecutor
{
public:
	HelloExecutor() = default;

	void init(const framework::utility::JSONSettingsParser::ExecutorSettings& settings) override;

	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doHead(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doTrace(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doConnect(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	~HelloExecutor() = default;
};
