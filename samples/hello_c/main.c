#include <stdio.h>

#include <import.h>

static void printServerState();

int main(int argc, char** argv)
{
	wf_initialize_web_framework("WebFramework"); // Load WebFramework shared library

	config_t config;
	web_framework_t server;
	web_framework_exception_t exception = wf_create_config_from_path("configs/config.json", &config); // Load config.json

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", wf_get_error_message(exception)); // Print error message

		wf_delete_web_framework_exception(exception); // Free WebFrameworkException memory

		return -1;
	}

	exception = wf_override_base_path(config, "executors"); // Override base path for loading executors

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", wf_get_error_message(exception)); // Print error message

		wf_delete_config(config); // Free Config memory
		wf_delete_web_framework_exception(exception); // Free WebFrameworkException memory

		return -2;
	}

	exception = wf_create_web_framework_from_config(config, &server); // Create server

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", wf_get_error_message(exception)); // Print error message

		wf_delete_config(config); // Free Config memory
		wf_delete_web_framework_exception(exception); // Free WebFrameworkException memory

		return -3;
	}

	exception = wf_start_web_framework_server(server, true, printServerState); // Start server and wait

	if (exception) // Check error
	{
		fprintf(stderr, "%s\n", wf_get_error_message(exception)); // Print error message

		wf_delete_config(config); // Free Config memory
		wf_delete_web_framework(server); // Free WebFramework memory
		wf_delete_web_framework_exception(exception); // Free WebFrameworkException memory

		return -4;
	}

	wf_delete_config(config); // Free Config memory
	wf_delete_web_framework(server); // Free WebFramework memory

	return 0;
}

void printServerState()
{
	printf("Server is running at http://127.0.0.1:8080\n");
}
