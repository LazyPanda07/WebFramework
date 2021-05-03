#include "BaseStatelessExecutor.h"

namespace framework
{
	BaseExecutor::executorType BaseStatelessExecutor::getType() const
	{
		return executorType::stateless;
	}

	void BaseStatelessExecutor::destroy()
	{

	}
}