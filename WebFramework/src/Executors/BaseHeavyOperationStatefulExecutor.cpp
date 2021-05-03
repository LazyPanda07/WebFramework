#include "BaseHeavyOperationStatefulExecutor.h"

using namespace std;

namespace framework
{
	BaseExecutor::executorType BaseHeavyOperationStatefullExecutor::getType() const
	{
		return executorType::heavyOperationStateful;
	}
}
