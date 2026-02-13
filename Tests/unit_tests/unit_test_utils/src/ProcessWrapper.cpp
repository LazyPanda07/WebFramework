#include "ProcessWrapper.h"

namespace unit_test_utils
{
	ProcessWrapper ProcessWrapper::runDefaultHttpsServer()
	{
		return ProcessWrapper("DefaultHTTPSServer");
	}

	ProcessWrapper::~ProcessWrapper()
	{
		process.kill();
	}
}
