#include "HTTPNetwork.h"

#include <algorithm>
#include <charconv>

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	HTTPNetwork::HTTPNetwork(SOCKET clientSocket) :
		web::Network(clientSocket)
	{

	}

	int HTTPNetwork::sendData(const vector<char>& data)
	{
		try
		{
			return web::Network::sendBytes(data.data(), data.size());
		}
		catch (const web::WebException&)
		{
			return -1;
		}
	}

	int HTTPNetwork::receiveData(vector<char>& data)
	{
		data.resize(averageHTTPRequestSize);
		int totalSize = 0;
		int lastPacket = 0;
		int bodySize = -1;
		bool isFindEnd = false;

		while (!isFindEnd)
		{
			if (totalSize >= data.size() - 100)
			{
				data.resize(data.size() * 2);
			}

			lastPacket = recv(web::Network::clientSocket, data.data() + totalSize, data.size() - totalSize, NULL);

			if (lastPacket == SOCKET_ERROR || !lastPacket)
			{
				throw web::WebException();
			}

			totalSize += lastPacket;

			transform(data.begin(), data.begin() + totalSize, data.begin(), [](char c) { return tolower(c); });

			string_view findHeader(data.data(), data.size());
			size_t contentLength = findHeader.find(contentLengthHeader);

			if (contentLength == string_view::npos)
			{
				isFindEnd = findHeader.find(crlfcrlf) != string_view::npos;
			}
			else
			{
				size_t endOfHTPP = findHeader.find(crlfcrlf);

				if (bodySize == -1)
				{
					size_t contentLengthHeaderPosition = contentLength + contentLengthHeader.size();
					string_view contentLengthValue = findHeader.substr(contentLengthHeaderPosition, findHeader.find("\r\n", contentLength) - contentLengthHeaderPosition);

					from_chars(contentLengthValue.data(), contentLengthValue.data() + contentLengthValue.size(), bodySize);

					data.resize(data.size() + bodySize);
				}

				if (endOfHTPP != string_view::npos)
				{
					isFindEnd = findHeader.size() == (endOfHTPP + crlfcrlf.size() + bodySize);
				}
			}
		}

		data.resize(totalSize);

		return totalSize;
	}
}