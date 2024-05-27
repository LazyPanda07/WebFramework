#include "ProxyServer.h"

#include "JSONArrayWrapper.h"

#include "Exceptions/SSLException.h"
#include "WebNetwork/WebFrameworkHTTPNetwork.h"
#include "WebNetwork/WebFrameworkHTTPSNetwork.h"

using namespace std;

namespace framework
{
	namespace proxy
	{
		ProxyServer::ProxyData::ProxyData(string_view ip, string_view port, DWORD timeout, bool isHTTPS) :
			BaseConnectionData(ip, port, timeout),
			isHTTPS(isHTTPS)
		{

		}

		void ProxyServer::clientConnection(const string& ip, SOCKET clientSocket, const sockaddr& addr, function<void()>&& cleanup)
		{
			SSL* ssl = nullptr;

			if (useHTTPS)
			{
				ssl = SSL_new(context);

				if (!ssl)
				{
					throw web::exceptions::SSLException(__LINE__, __FILE__);
				}

				if (!SSL_set_fd(ssl, static_cast<int>(clientSocket)))
				{
					SSL_free(ssl);

					throw web::exceptions::SSLException(__LINE__, __FILE__);
				}

				if (int errorCode = SSL_accept(ssl); errorCode != 1)
				{
					throw web::exceptions::SSLException(__LINE__, __FILE__, ssl, errorCode);
				}
			}

			streams::IOSocketStream clientStream
			(
				ssl ?
				make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPSNetwork>(clientSocket, ssl, context)) :
				make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPNetwork>(clientSocket))
			);

			string request;
			string response;

			clientStream >> request;

			web::HTTPParser parser(request);

			string route = parser.getParameters();

			if (route.find('?') != string::npos)
			{
				route.resize(route.find('?'));
			}

			const ProxyData& data = *routes.at(route);

			streams::IOSocketStream serverStream
			(
				data.isHTTPS ?
				std::make_unique<web::HTTPSNetwork>(data.ip, data.port, data.timeout) :
				std::make_unique<web::HTTPNetwork>(data.ip, data.port, data.timeout)
			);

			serverStream << request;

			serverStream >> response;

			clientStream << response;
		}

		ProxyServer::ProxyServer(string_view ip, string_view port, DWORD timeout, const json::utility::jsonObject& proxySettings) :
			BaseTCPServer
			(
				port,
				ip,
				timeout,
				true,
				0,
				false
			)
		{
			const vector<json::utility::jsonObject>& servers = proxySettings.getArray("proxiedServers");

			proxyData.reserve(servers.size());

			for (const json::utility::jsonObject& proxiedServer : servers)
			{
				const string& ip = proxiedServer.getString("ip");
				int64_t port = proxiedServer.getInt("port");
				uint64_t timeout = proxiedServer.getUnsignedInt("timeout");
				bool isHTTPS = proxiedServer.getBool("isHTTPS");
				vector<string> serverRoutes = json::utility::JSONArrayWrapper(proxiedServer.getArray("routes")).getAsStringArray();

				const ProxyData& data = proxyData.emplace_back(ip, to_string(port), static_cast<DWORD>(timeout), isHTTPS);

				for (const string& route : serverRoutes)
				{
					routes[route] = &data;
				}
			}
		}
	}
}
