#include <stdio.h>
#include <stdlib.h>

#include "import.h"

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

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

	exception = overrideConfigurationInteger(config, "port", atoi(argv[2]), true);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -2;
	}

	for (int i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "--useHTTPS"))
		{
			exception = overrideConfigurationBoolean(config, "useHTTPS", true, true);

			if (exception)
			{
				printf("%s\n", getErrorMessage(exception));

				return -3;
			}	
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
