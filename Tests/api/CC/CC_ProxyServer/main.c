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

	web_framework_t server;
	config_t config;
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

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -1;
	}

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "--useHTTPS"))
		{
			exception = wf_override_configuration_boolean(config, "useHTTPS", true, true);

			if (exception)
			{
				printf("%s\n", wf_get_error_message(exception));

				return -2;
			}	
		}
		else if (!strcmp(argv[i], "--port"))
		{
			port = atoi(argv[i + 1]);

			exception = wf_override_configuration_integer(config, "port", port, true);

			if (exception)
			{
				printf("%s\n", wf_get_error_message(exception));

				return -3;
			}

			i++;
		}
	}

	exception = wf_create_web_framework_from_config(config, &server);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -4;
	}

	exception = wf_start_web_framework_server(server, true, printRunningState);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -5;
	}

	return 0;
}
