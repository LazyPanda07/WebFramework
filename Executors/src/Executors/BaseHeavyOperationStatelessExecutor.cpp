#include "BaseHeavyOperationStatelessExecutor.h"

using namespace std;

namespace framework
{
	BaseExecutor::ExecutorType BaseHeavyOperationStatelessExecutor::getType() const
	{
		return ExecutorType::heavyOperationStateless;
	}

	void BaseHeavyOperationStatelessExecutor::destroy()
	{

	}
}
