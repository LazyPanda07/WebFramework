#include <stdio.h>
#include <stdlib.h>

#include "import.h"

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

	initializeWebFramework("WebFramework");

	WebFramework server;
	WebFrameworkException exception = createWebFrameworkFromPath(argv[1], &server);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -1;
	}

	exception = startWebFrameworkServer(server, true, writeProcessId);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -2;
	}

	return 0;
}
