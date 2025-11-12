#include "ProxyServer.h"

#include <Exceptions/SslException.h>
#include <HttpsNetwork.h>
#include <JsonArrayWrapper.h>
#include <IOSocketStream.h>

namespace framework::proxy
{
	ProxyServer::ProxyData::ProxyData(std::string_view ip, std::string_view port, DWORD timeout, bool isHTTPS) :
		BaseConnectionData(ip, port, timeout),
		isHTTPS(isHTTPS)
	{

	}

	void ProxyServer::clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr addr, std::function<void()>& cleanup) //-V688
	{
		SSL* ssl = nullptr;

		if (useHTTPS)
		{
			ssl = SSL_new(context);

			if (!ssl)
			{
				throw web::exceptions::SslException(__LINE__, __FILE__);
			}

			if (!SSL_set_fd(ssl, static_cast<int>(clientSocket)))
			{
				SSL_free(ssl);

				throw web::exceptions::SslException(__LINE__, __FILE__);
			}

			if (int errorCode = SSL_accept(ssl); errorCode != 1)
			{
				throw web::exceptions::SslException(__LINE__, __FILE__, ssl, errorCode);
			}
		}

		streams::IOSocketStream clientStream = ssl ?
			streams::IOSocketStream::createStream<web::HttpsNetwork>(clientSocket, ssl, context, std::chrono::milliseconds(timeout)) :
			streams::IOSocketStream::createStream<web::HttpNetwork>(clientSocket, std::chrono::milliseconds(timeout));

		std::string request;
		std::string response;

		clientStream >> request;

		web::HttpParser parser(request);

		std::string route = parser.getParameters();

		if (route.find('?') != std::string::npos)
		{
			route.resize(route.find('?'));
		}

		const ProxyData& proxyData = *routes.at(route);

		streams::IOSocketStream serverStream = proxyData.isHTTPS ?
			streams::IOSocketStream::createStream<web::HttpsNetwork>(proxyData.ip, proxyData.port, std::chrono::milliseconds(proxyData.timeout)) :
			streams::IOSocketStream::createStream<web::HttpNetwork>(proxyData.ip, proxyData.port, std::chrono::milliseconds(proxyData.timeout));

		serverStream << request;

		serverStream >> response;

		clientStream << response;
	}

	ProxyServer::ProxyServer(std::string_view ip, std::string_view port, DWORD timeout, const json::JsonObject& proxySettings) :
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
		const std::vector<json::JsonObject>& servers = proxySettings["proxiedServers"].get<std::vector<json::JsonObject>>();

		proxyData.reserve(servers.size());

		for (const json::JsonObject& proxiedServer : servers)
		{
			const std::string& ip = proxiedServer["ip"].get<std::string>();
			int64_t port = proxiedServer["port"].get<int64_t>();
			uint64_t timeout = proxiedServer["timeout"].get<uint64_t>();
			bool isHTTPS = proxiedServer["isHTTPS"].get<bool>();
			std::vector<std::string> serverRoutes = json::utility::JsonArrayWrapper(proxiedServer["routes"].get<std::vector<json::JsonObject>>()).as<std::string>();

			const ProxyData& data = proxyData.emplace_back(ip, std::to_string(port), static_cast<DWORD>(timeout), isHTTPS);

			for (const std::string& route : serverRoutes)
			{
				routes[route] = &data;
			}
		}
	}
}
