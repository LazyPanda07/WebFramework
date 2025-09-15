#include "BaseHeavyOperationStatelessExecutor.h"

using namespace std;

namespace framework
{
	utility::ExecutorType BaseHeavyOperationStatelessExecutor::getType() const
	{
		return utility::ExecutorType::heavyOperationStateless;
	}

	void BaseHeavyOperationStatelessExecutor::destroy()
	{

	}
}
