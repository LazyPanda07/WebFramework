#include <stdio.h>
#include <stdlib.h>

#include "import.h"

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

int port;

void writeProcessId()
{
#ifdef __LINUX__
	pid_t processId = getpid();
#else
	DWORD processId = GetCurrentProcessId();
#endif

	FILE* file = NULL;

	switch (port)
	{
	case 15000:
		file = fopen(START_PROXY_SERVER_FILE, "w");

		break;

	case 15001:
		file = fopen(START_PROXY_HTTPS_SERVER_FILE, "w");

		break;

	default:
		break;
	}

	if (file)
	{
		fwrite(&processId, sizeof(processId), 1, file);

		fclose(file);
	}
}

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

	port = atoi(argv[2]);

	exception = overrideConfigurationInteger(config, "port", port, true);

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

	exception = startWebFrameworkServer(server, true, writeProcessId);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -5;
	}

	return 0;
}
