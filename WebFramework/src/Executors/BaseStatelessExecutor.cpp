#include "BaseStatelessExecutor.h"

namespace framework
{
	BaseExecutor::executorType BaseStatelessExecutor::getType() const
	{
		return BaseExecutor::executorType::stateless;
	}

	void BaseStatelessExecutor::destroy()
	{

	}
}