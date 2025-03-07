#include "BaseStatefulExecutor.h"

using namespace std;

namespace framework
{
	BaseExecutor::ExecutorType BaseStatefulExecutor::getType() const
	{
		return ExecutorType::stateful;
	}

	void BaseStatefulExecutor::destroy()
	{
		
	}
}
