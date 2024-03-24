#include <Executors/BaseStatelessExecutor.h>

class HelloExecutor : public framework::BaseStatelessExecutor
{
public:
	void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;
};
