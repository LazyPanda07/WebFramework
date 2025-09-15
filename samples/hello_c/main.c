#include <stdio.h>

#include <import.h>

static void printServerState();

int main(int argc, char** argv)
{
	initializeWebFramework("WebFramework"); // Load WebFramework shared library

	Config config;
	WebFramework server;
	WebFrameworkException exception = createConfigFromPath("configs/config.json", &config); // Load config.json

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", getErrorMessage(exception)); // Print error message

		deleteWebFrameworkException(exception); // Free WebFrameworkException memory

		return -1;
	}

	exception = overrideBasePath(config, "executors"); // Override base path for loading executors

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", getErrorMessage(exception)); // Print error message

		deleteWebFrameworkConfig(config); // Free Config memory
		deleteWebFrameworkException(exception); // Free WebFrameworkException memory

		return -2;
	}

	exception = createWebFrameworkFromConfig(config, &server); // Create server

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", getErrorMessage(exception)); // Print error message

		deleteWebFrameworkConfig(config); // Free Config memory
		deleteWebFrameworkException(exception); // Free WebFrameworkException memory

		return -3;
	}

	exception = startWebFrameworkServer(server, true, printServerState); // Start server and wait

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", getErrorMessage(exception)); // Print error message

		deleteWebFrameworkConfig(config); // Free Config memory
		deleteWebFramework(server); // Free WebFramework memory
		deleteWebFrameworkException(exception); // Free WebFrameworkException memory

		return -4;
	}

	deleteWebFrameworkConfig(config); // Free Config memory
	deleteWebFramework(server); // Free WebFramework memory

	return 0;
}

void printServerState()
{
	printf("Server is running at http://127.0.0.1:8080\n");
}
