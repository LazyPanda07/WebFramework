#include "StatefulExecutor.h"

namespace framework
{
	utility::ExecutorType StatefulExecutor::getType() const
	{
		return utility::ExecutorType::stateful;
	}

	void StatefulExecutor::destroy()
	{
		
	}
}
