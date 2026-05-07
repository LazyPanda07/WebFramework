#include <TaskBroker/TaskExecutor.hpp>

class CXXTestTaskExecutor : public framework::task_broker::TaskExecutor
{
public:
	CXXTestTaskExecutor() = default;

	void execute(const framework::JsonObject& data, const TaskExecutorContext& context) override;

	~CXXTestTaskExecutor() = default;
};
