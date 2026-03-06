#include <TaskBroker/TaskExecutor.hpp>

class CXXTestTaskExecutor : public framework::task_broker::TaskExecutor
{
public:
	CXXTestTaskExecutor() = default;

	void operator ()(const framework::JsonObject& data) override;

	~CXXTestTaskExecutor() = default;
};
