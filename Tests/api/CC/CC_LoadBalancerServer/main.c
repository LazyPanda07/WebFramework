#include <stdio.h>
#include <stdlib.h>

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

#include "import.h"

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
	case 9090:
		file = fopen(START_LOAD_BALANCER_9090_SERVER_FILE, "w");

		break;

	case 9091:
		file = fopen(START_LOAD_BALANCER_9091_SERVER_FILE, "w");

		break;

	case 9092:
		file = fopen(START_LOAD_BALANCER_9092_SERVER_FILE, "w");

		break;

	case 9093:
		file = fopen(START_LOAD_BALANCER_9093_SERVER_FILE, "w");

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

	Config config;
	WebFramework server;

	WebFrameworkException exception = createConfigFromPath(argv[1], &config);
	bool serversHTTPS = false;
	const char* type = "";

	for (int i = 2; i < argc;)
	{
		if (!strcmp(argv[i], "--port"))
		{
			port = atoi(argv[i + 1]);

			exception = overrideConfigurationInteger(config, "port", port, true);

			if (exception)
			{
				printf("%s\n", getErrorMessage(exception));

				return -1;
			}

			i += 2;
		}
		else if (!strcmp(argv[i], "--serversHTTPS"))
		{
			serversHTTPS = true;

			i++;
		}
		else if (!strcmp(argv[i], "--custom_heuristic"))
		{
			exception = overrideConfigurationString(config, "heuristic", "CustomHeuristic", true);

			if (exception)
			{
				printf("%s\n", getErrorMessage(exception));

				return -2;
			}

			i++;
		}
		else if (!strcmp(argv[i], "--type"))
		{
			type = argv[i + 1];

			i += 2;
		}
	}

	if (!strcmp(type, "server"))
	{
		const char** settingsPaths = malloc(sizeof(const char*));

		settingsPaths[0] = "load_balancer_web.json";

		exception = overrideConfigurationString(config, "webServerType", "multiThreaded", true);

		if (exception)
		{
			printf("%s\n", getErrorMessage(exception));

			return -3;
		}

		exception = overrideConfigurationStringArray(config, "settingsPaths", settingsPaths, true, 1);

		if (exception)
		{
			printf("%s\n", getErrorMessage(exception));

			return -4;
		}

		free(settingsPaths);
	}
	else
	{
		int64_t* listOfServers = malloc(sizeof(int64_t) * 2);

		exception = overrideConfigurationBoolean(config, "serversHTTPS", serversHTTPS, true);

		if (exception)
		{
			printf("%s\n", getErrorMessage(exception));

			return -5;
		}

		if (serversHTTPS)
		{
			listOfServers[0] = 10002;
			listOfServers[1] = 10003;
		}
		else
		{
			listOfServers[0] = 10000;
			listOfServers[1] = 10001;
		}

		exception = overrideConfigurationIntegerArray(listOfServers, "127.0.0.1", listOfServers, true, 2);

		if (exception)
		{
			printf("%s\n", getErrorMessage(exception));

			return -6;
		}

		free(listOfServers);
	}

	exception = createWebFrameworkFromConfig(config, &server);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -7;
	}

	exception = startWebFrameworkServer(server, true, writeProcessId);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -8;
	}

	return 0;
}
