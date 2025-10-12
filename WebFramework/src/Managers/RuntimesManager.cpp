#include "RuntimesManager.h"

namespace framework::runtime
{
	RuntimesManager& RuntimesManager::get()
	{
		static RuntimesManager instance;

		return instance;
	}
}
