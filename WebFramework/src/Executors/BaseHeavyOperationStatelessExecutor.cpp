#include "BaseHeavyOperationStatelessExecutor.h"

using namespace std;

namespace framework
{
	BaseExecutor::executorType BaseHeavyOperationStatelessExecutor::getType() const
	{
		return executorType::heavyOperationStateless;
	}

	void BaseHeavyOperationStatelessExecutor::destroy()
	{

	}
}
