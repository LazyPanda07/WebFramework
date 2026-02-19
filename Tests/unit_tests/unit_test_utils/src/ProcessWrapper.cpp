#include "ProcessWrapper.h"

namespace unit_test_utils
{
	ProcessWrapper ProcessWrapper::runDefaultHttpsServer()
	{
#ifdef __LINUX_AARCH64__
		return ProcessWrapper("qemu-aarch64 ./DefaultHTTPSServer");
#else
		return ProcessWrapper("./DefaultHTTPSServer");
#endif
	}

	ProcessWrapper::~ProcessWrapper()
	{
		process.kill();
	}
}
