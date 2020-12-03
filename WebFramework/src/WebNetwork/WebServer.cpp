#include "WebServer.h"

#include "HTTPNetwork.h"
#include "BaseIOSocketStream.h"
#include "Exceptions/NotImplementedException.h"

using namespace std;

namespace framework
{
	void WebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream stream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
		const string clientIp = getIpV4(addr);
		string response;

		while (true)
		{
			try
			{
				string HTTPRequest;

				response.clear();

				stream >> HTTPRequest;

				if (HTTPRequest.find("HTTP") != string::npos)
				{
					manager.service(web::HTTPParser(HTTPRequest), response);

					stream << response;
				}
				else
				{
					throw web::WebException();
				}
			}
			catch (const web::WebException&)
			{
				data.erase(clientIp);

				break;
			}
			catch (const exceptions::NotImplementedException&)
			{
				// 400
			}
			catch (const exceptions::BaseExecutorException&)
			{

			}
			catch (const out_of_range&)
			{
				// 404
			}
		}
	}

	WebServer::WebServer(ExecutorsManager&& manager, const string_view& port, DWORD timeout) :
		BaseTCPServer(port, timeout, false),
		manager(move(manager))
	{

	}
}