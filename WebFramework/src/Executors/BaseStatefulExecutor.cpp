#include "BaseStatefulExecutor.h"

using namespace std;

namespace framework
{
	BaseExecutor::executorType BaseStatefulExecutor::getType() const
	{
		return BaseExecutor::executorType::stateful;
	}
}