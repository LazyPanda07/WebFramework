#include "BaseStatelessExecutor.h"

namespace framework
{
	BaseExecutor::ExecutorType BaseStatelessExecutor::getType() const
	{
		return ExecutorType::stateless;
	}

	void BaseStatelessExecutor::destroy()
	{

	}
}