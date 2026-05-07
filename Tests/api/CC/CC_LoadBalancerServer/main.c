#include <stdio.h>
#include <stdlib.h>

#include <import.h>

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

int port;

void printRunningState()
{
	printf("Server is running...\n");
	fflush(stdout);
}

int main(int argc, char** argv)
{
	wf_initialize_web_framework("WebFramework");

	config_t config;
	web_framework_t server;
	const char* configName = NULL;

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "--config"))
		{
			configName = argv[i + 1];

			break;
		}
	}

	web_framework_exception_t exception = wf_create_config_from_path(configName, &config);
	bool serversHTTPS = false;
	const char* type = "";

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -1;
	}

	char* temp = getenv("RUNTIMES");

	if (temp)
	{
		if (strstr(temp, "python"))
		{
			wf_override_configuration_boolean(config, "$[]WebFramework.runtimes.0.enabled", true, false);
		}

		if (strstr(temp, ".net"))
		{
			wf_override_configuration_boolean(config, "$[]WebFramework.runtimes.1.enabled", true, false);
		}
	}

	for (int i = 2; i < argc; i++)
	{
		if (!strcmp(argv[i], "--port"))
		{
			port = atoi(argv[i + 1]);

			exception = wf_override_configuration_integer(config, "port", port, true);

			if (exception)
			{
				printf("%s\n", wf_get_error_message(exception));

				return -2;
			}

			i++;
		}
		else if (!strcmp(argv[i], "--serversHTTPS"))
		{
			serversHTTPS = true;
		}
		else if (!strcmp(argv[i], "--custom_heuristic"))
		{
			exception = wf_override_configuration_string(config, "$[]LoadBalancer.heuristic.name", "CustomHeuristic", true);

			if (exception)
			{
				printf("%s\n", wf_get_error_message(exception));

				return -3;
			}
		}
		else if (!strcmp(argv[i], "--type"))
		{
			type = argv[i + 1];

			i++;
		}
	}

	if (!strcmp(type, "server"))
	{
		const char** settingsPaths = malloc(sizeof(const char*));

		if (!settingsPaths)
		{
			return -4;
		}

		settingsPaths[0] = "load_balancer_web.json";

		exception = wf_override_configuration_string(config, "webServerType", "multiThreaded", true);

		if (exception)
		{
			printf("%s\n", wf_get_error_message(exception));

			return -5;
		}

		exception = wf_override_configuration_string_array(config, "settingsPaths", settingsPaths, true, 1);

		if (exception)
		{
			printf("%s\n", wf_get_error_message(exception));

			return -6;
		}

		free(settingsPaths);
	}
	else
	{
		int64_t* listOfServers = malloc(sizeof(int64_t) * 2);

		if (!listOfServers)
		{
			return -7;
		}

		exception = wf_override_configuration_boolean(config, "serversHTTPS", serversHTTPS, true);

		if (exception)
		{
			printf("%s\n", wf_get_error_message(exception));

			return -8;
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

		exception = wf_override_configuration_integer_array(config, "127.0.0.1", listOfServers, true, 2);

		if (exception)
		{
			printf("%s\n", wf_get_error_message(exception));

			return -9;
		}

		free(listOfServers);
	}

	exception = wf_create_web_framework_from_config(config, &server);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -10;
	}

	exception = wf_start_web_framework_server(server, true, printRunningState);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -11;
	}

	return 0;
}
