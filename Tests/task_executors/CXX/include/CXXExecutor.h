#include <Executors/StatelessExecutor.hpp>

class CXXExecutor : public framework::StatelessExecutor
{
public:
	void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
