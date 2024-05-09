#include "BaseHeavyOperationStatefulExecutor.h"

using namespace std;

namespace framework
{
	BaseExecutor::executorType BaseHeavyOperationStatefulExecutor::getType() const
	{
		return executorType::heavyOperationStateful;
	}

	void BaseHeavyOperationStatefulExecutor::destroy()
	{

	}
}
