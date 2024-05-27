#include "WebFramework.h"

#include "ConsoleArgumentParser.h"

int main(int argc, char** argv)
{
	utility::parsers::ConsoleArgumentParser parser(argc, argv);
	framework::utility::Config config(parser.get<std::string>("config"));

	config.overrideConfiguration("useHTTPS", parser.get<bool>("useHTTPS"), true);
	config.overrideConfiguration("port", parser.get<int64_t>("port"), true);

	framework::WebFramework server(config);

	server.startServer(true);

	return 0;
}
