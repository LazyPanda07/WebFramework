#include <stdio.h>
#include <stdlib.h>

#include "import.h"

int main(int argc, char** argv)
{
	initializeWebFramework("WebFramework");

	WebFramework server;
	WebFrameworkException exception = createWebFrameworkFromPath("config.json", &server);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -1;
	}

	exception = startWebFrameworkServer(server, true, NULL);

	if (exception)
	{
		printf("%s\n", getErrorMessage(exception));

		return -2;
	}

	return 0;
}
