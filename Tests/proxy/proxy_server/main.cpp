#include "WebFramework.h"

#include "ConsoleArgumentParser.h"

int main(int argc, char** argv) try
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);
	framework::utility::Config config(parser.get<std::string>("--config"));

	config.overrideConfiguration("useHTTPS", parser.get<bool>("--useHTTPS"), true);
	config.overrideConfiguration("port", parser.get<int64_t>("--port"), true);

	framework::WebFramework server(config);

	server.startServer(true);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
