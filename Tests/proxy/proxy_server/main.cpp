#include "WebFramework.h"

#include "ConsoleArgumentParser.h"

int main(int argc, char** argv) try
{
	// start ProxyServer.exe --config proxy_config.json --port 15000

	argc = 5;
	argv = new char*[argc];

	std::string str = (std::filesystem::current_path() / "ProxyServer.exe").string();

	argv[0] = str.data();
	argv[1] = const_cast<char*>("--config");
	argv[2] = const_cast<char*>("proxy_config.json");
	argv[3] = const_cast<char*>("--port");
	argv[4] = const_cast<char*>("15000");

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
