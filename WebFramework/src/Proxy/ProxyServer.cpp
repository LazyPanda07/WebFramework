#include "ProxyServer.h"

#include "JSONArrayWrapper.h"
#include "HTTPSNetwork.h"

#include "Exceptions/SSLException.h"

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

		void ProxyServer::clientConnection(const string& ip, SOCKET clientSocket, const sockaddr& addr, function<void()>&& cleanup) //-V688
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
				make_unique<web::HTTPSNetwork>(clientSocket, ssl, context) :
				make_unique<web::HTTPNetwork>(clientSocket)
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

			const ProxyData& proxyData = *routes.at(route);

			streams::IOSocketStream serverStream
			(
				proxyData.isHTTPS ?
				make_unique<web::HTTPSNetwork>(proxyData.ip, proxyData.port, proxyData.timeout) :
				make_unique<web::HTTPNetwork>(proxyData.ip, proxyData.port, proxyData.timeout)
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
			vector<json::utility::jsonObject> servers = json::utility::JSONArrayWrapper(proxySettings.getArray("proxiedServers")).getAsObjectArray();

			proxyData.reserve(servers.size());

			for (const json::utility::jsonObject& proxiedServer : servers)
			{
				const string& ip = proxiedServer.getString("ip"); //-V688
				int64_t port = proxiedServer.getInt("port"); //-V688
				uint64_t timeout = proxiedServer.getUnsignedInt("timeout"); //-V688
				bool isHTTPS = proxiedServer.getBool("isHTTPS");
				vector<string> serverRoutes = json::utility::JSONArrayWrapper(proxiedServer.getArray("routes")).getAsStringArray();

				const ProxyData& data = proxyData.emplace_back(ip, to_string(port), static_cast<DWORD>(timeout), isHTTPS); //-V688

				for (const string& route : serverRoutes)
				{
					routes[route] = &data;
				}
			}
		}
	}
}
