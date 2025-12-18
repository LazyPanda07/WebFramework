#include <stdio.h>
#include <stdlib.h>

#include <import.h>

#ifdef __LINUX__
#include <unistd.h> 
#else
#include <Windows.h>
#endif

void writeProcessId()
{
#ifdef __LINUX__
	pid_t processId = getpid();
#else
	DWORD processId = GetCurrentProcessId();
#endif

	FILE* file = fopen(START_CORE_SERVER_FILE, "w");

	fwrite(&processId, sizeof(processId), 1, file);

	fclose(file);
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

	web_framework_t server;
	web_framework_exception_t exception = wf_create_web_framework_from_path(argv[1], &server);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -1;
	}

	exception = wf_start_web_framework_server(server, true, writeProcessId);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -2;
	}

	return 0;
}
