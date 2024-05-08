#include "BaseStatefulExecutor.h"

using namespace std;

namespace framework
{
	BaseExecutor::executorType BaseStatefulExecutor::getType() const
	{
		return executorType::stateful;
	}

	void BaseStatefulExecutor::destroy()
	{
		
	}
}
