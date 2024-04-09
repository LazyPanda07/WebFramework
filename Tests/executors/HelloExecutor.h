#include <Executors/BaseStatelessExecutor.h>

class HelloExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPost(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doHead(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPut(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doDelete(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doOptions(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doTrace(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

	void doPatch(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
