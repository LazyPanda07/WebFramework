#include <stdio.h>
#include <stdlib.h>

#include "import.h"

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		wf_initialize_web_framework(argv[1]);
	}
	else
	{
		wf_initialize_web_framework(NULL);
	}

	web_framework_t server;
	web_framework_exception_t exception = wf_create_web_framework_from_path("config.json", &server);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -1;
	}

	exception = wf_start_web_framework_server(server, true, NULL);

	if (exception)
	{
		printf("%s\n", wf_get_error_message(exception));

		return -2;
	}

	return 0;
}
