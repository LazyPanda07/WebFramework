#include <stdio.h>
#include <stdlib.h>

#include <import.h>

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

void printRunningState()
{
	printf("Server is running...\n");
	fflush(stdout);
}

int main(int argc, char** argv)
{
	{
		FILE* database = fopen("test_database.sqlite", "rb");

		if (database)
		{
			fclose(database);

			remove("test_database.sqlite");
		}
	}

	wf_initialize_web_framework("WebFramework");

	config_t config;
	web_framework_t server;
	web_framework_exception_t exception = wf_create_config_from_path(argv[1], &config);

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

	exception = wf_create_web_framework_from_config(config, &server);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -2;
	}

	exception = wf_start_web_framework_server(server, true, printRunningState);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -3;
	}

	return 0;
}
