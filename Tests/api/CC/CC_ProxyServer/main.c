#include <stdio.h>
#include <stdlib.h>

#include <import.h>

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
	wf_initialize_web_framework("WebFramework");

	web_framework_t server;
	config_t config;
	web_framework_exception_t exception = wf_create_config_from_path(argv[1], &config);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -1;
	}

	port = atoi(argv[2]);

	exception = wf_override_configuration_integer(config, "port", port, true);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -2;
	}

	for (int i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "--useHTTPS"))
		{
			exception = wf_override_configuration_boolean(config, "useHTTPS", true, true);

			if (exception)
			{
				printf("%s\n", wf_get_error_message(exception));

				return -3;
			}	
		}
	}

	exception = wf_create_web_framework_from_config(config, &server);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -4;
	}

	exception = wf_start_web_framework_server(server, true, writeProcessId);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -5;
	}

	return 0;
}
