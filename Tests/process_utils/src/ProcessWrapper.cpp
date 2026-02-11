#include "ProcessWrapper.h"

namespace process_utils
{
	ProcessWrapper::~ProcessWrapper()
	{
		process.kill();
	}
}
