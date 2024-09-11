#include <stdio.h>

#include <import.h>

int main(int argc, char** argv)
{
	initializeWebFramework("WebFramework"); // Load WebFramework shared library

	Config config;
	WebFramework server;
	WebFrameworkException exception = createConfigFromPath("configs/config.json", &config); // Load config.json

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", getErrorMessage(exception)); // Print error message

		deleteWebFrameworkObject(exception); // Free WebFrameworkException memory

		return -1;
	}

	exception = overrideBasePath(config, "executors"); // Override base path for loading executors

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", getErrorMessage(exception)); // Print error message

		deleteWebFrameworkObject(config); // Free Config memory
		deleteWebFrameworkObject(exception); // Free WebFrameworkException memory

		return -2;
	}

	exception = createWebFrameworkFromConfig(config, &server); // Create server

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", getErrorMessage(exception)); // Print error message

		deleteWebFrameworkObject(config); // Free Config memory
		deleteWebFrameworkObject(exception); // Free WebFrameworkException memory

		return -3;
	}

	exception = startWebFrameworkServer(server, true, NULL); // Start server and wait

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", getErrorMessage(exception)); // Print error message

		deleteWebFrameworkObject(config); // Free Config memory
		deleteWebFrameworkObject(server); // Free WebFramework memory
		deleteWebFrameworkObject(exception); // Free WebFrameworkException memory

		return -4;
	}

	deleteWebFrameworkObject(config); // Free Config memory
	deleteWebFrameworkObject(server); // Free WebFramework memory

	return 0;
}
