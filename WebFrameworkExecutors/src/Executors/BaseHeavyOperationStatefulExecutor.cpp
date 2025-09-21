#include "BaseHeavyOperationStatefulExecutor.h"

namespace framework
{
	utility::ExecutorType BaseHeavyOperationStatefulExecutor::getType() const
	{
		return utility::ExecutorType::heavyOperationStateful;
	}

	void BaseHeavyOperationStatefulExecutor::destroy()
	{

	}
}
