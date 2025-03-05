#include "BaseHeavyOperationStatefulExecutor.h"

using namespace std;

namespace framework
{
	BaseExecutor::ExecutorType BaseHeavyOperationStatefulExecutor::getType() const
	{
		return ExecutorType::heavyOperationStateful;
	}

	void BaseHeavyOperationStatefulExecutor::destroy()
	{

	}
}
