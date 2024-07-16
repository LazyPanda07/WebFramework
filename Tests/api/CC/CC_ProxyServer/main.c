#include <stdio.h>
#include <stdlib.h>

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

#include "import.h"

int main(int argc, char** argv)
{
	initializeWebFramework("WebFramework");

	WebFramework server;
	Config config;
	WebFrameworkException exception = createConfigFromPath(argv[1], &config);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -1;
	}

	if (argc > 2)
	{
		exception = overrideConfigurationInteger(config, "port", atoi(argv[2]), true);

		if (exception)
		{
			printf("%s\n", getErrorMessage(exception));

			return -2;
		}
	}

	if (argc == 3)
	{
		exception = overrideConfigurationInteger(config, "useHTTPS", true, true);

		if (exception)
		{
			printf("%s\n", getErrorMessage(exception));

			return -3;
		}
	}

	exception = createWebFrameworkFromConfig(config, &server);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -4;
	}

	exception = startWebFrameworkServer(server, true, NULL);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -5;
	}

	return 0;
}
