#include "BaseStatefulExecutor.h"

namespace framework
{
	utility::ExecutorType BaseStatefulExecutor::getType() const
	{
		return utility::ExecutorType::stateful;
	}

	void BaseStatefulExecutor::destroy()
	{
		
	}
}
