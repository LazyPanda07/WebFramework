#include <Executors/HeavyOperationStatelessExecutor.hpp>

class TokenExecutor : public framework::HeavyOperationStatelessExecutor
{
public:
	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
