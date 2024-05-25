#include "WebFramework.h"

int main(int argc, char** argv) try
{
	framework::WebFramework server(argv[1]);

	server.startServer(true);

	return 0;
}
catch (const web::exceptions::WebException& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
