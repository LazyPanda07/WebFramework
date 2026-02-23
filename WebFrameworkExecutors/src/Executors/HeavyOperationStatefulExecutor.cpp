#include "HeavyOperationStatefulExecutor.h"

namespace framework
{
	utility::ExecutorType HeavyOperationStatefulExecutor::getType() const
	{
		return utility::ExecutorType::heavyOperationStateful;
	}

	void HeavyOperationStatefulExecutor::destroy()
	{

	}
}
