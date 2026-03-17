#include "Executors/StatelessExecutor.h"

namespace framework
{
	utility::ExecutorType StatelessExecutor::getType() const
	{
		return utility::ExecutorType::stateless;
	}

	void StatelessExecutor::destroy()
	{

	}
}
