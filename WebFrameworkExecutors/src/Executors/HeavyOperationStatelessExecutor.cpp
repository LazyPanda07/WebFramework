#include "Executors/HeavyOperationStatelessExecutor.h"

namespace framework
{
	utility::ExecutorType HeavyOperationStatelessExecutor::getType() const
	{
		return utility::ExecutorType::heavyOperationStateless;
	}

	void HeavyOperationStatelessExecutor::destroy()
	{

	}
}
