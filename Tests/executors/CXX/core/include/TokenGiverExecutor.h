#include <Executors/StatelessExecutor.hpp>

class TokenGiverExecutor : public framework::StatelessExecutor
{
public:
	void doPost(framework::HttpRequest& request, framework::HttpResponse& response) override;
};
