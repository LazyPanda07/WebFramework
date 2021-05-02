#include "ThreadPoolWebServer.h"

using namespace std;

namespace framework
{
	void ThreadPoolWebServer::receiveConnections()
	{
		BaseTCPServer::receiveConnections();


	}

	void ThreadPoolWebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{

	}

	ThreadPoolWebServer::ThreadPoolWebServer(const vector<utility::JSONSettingsParser>& parsers, const filesystem::path& assets, const string& pathToTemplates, bool isCaching, const string& ip, const string& port, DWORD timeout, const vector<string>& pathToSources, uint32_t threadCount) :
		BaseTCPServer
		(
			port,
			ip,
			timeout,
			false,
			1,
			false
		),
		BaseWebServer
		(
			parsers,
			assets,
			pathToTemplates,
			isCaching,
			ip,
			port,
			timeout,
			pathToSources
		),
		threadPool(threadCount ? threadCount : thread::hardware_concurrency())
	{

	}
}
