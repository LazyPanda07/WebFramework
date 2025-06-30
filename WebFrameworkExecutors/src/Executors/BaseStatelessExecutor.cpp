#include "BaseStatelessExecutor.h"

namespace framework
{
	utility::ExecutorType BaseStatelessExecutor::getType() const
	{
		return utility::ExecutorType::stateless;
	}

	void BaseStatelessExecutor::destroy()
	{

	}
}
