#include "HTTPRequest.h"

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	bool HTTPRequest::isWebFrameworkDynamicPages(const string& filePath)
	{
		size_t extension = filePath.find('.');

		if (extension == string::npos)
		{
			return false;
		}

		return string_view(filePath.data() + extension) == webFrameworkDynamicPagesExtension;
	}

	HTTPRequest::HTTPRequest(SessionsManager& session, const string& ip, interfaces::ISendStaticFile& staticResources, interfaces::ISendDynamicFile& dynamicResources) :
		session(session),
		ip(ip),
		staticResources(staticResources),
		dynamicResources(dynamicResources)
	{
		
	}

	string HTTPRequest::getRawParameters() const
	{
		return parser->getParameters();
	}

	string HTTPRequest::getMethod() const
	{
		return parser->getMethod();
	}

	const unordered_map<string, string>& HTTPRequest::getKeyValueParameters() const
	{
		return parser->getKeyValueParameters();
	}

	const string& HTTPRequest::getHTTPVersion() const
	{
		return parser->getHTTPVersion();
	}

	const unordered_map<string, string>& HTTPRequest::getHeaders() const
	{
		return parser->getHeaders();
	}

	const string& HTTPRequest::getBody() const
	{
		return parser->getBody();
	}

	void HTTPRequest::setAttribute(const string& name, const string& value)
	{
		session.setAttribute(ip, name, value);
	}

	string HTTPRequest::getAttribute(const string& name)
	{
		return session.getAttribute(ip, name);
	}

	void HTTPRequest::deleteSession()
	{
		session.deleteSession(ip);
	}

	void HTTPRequest::deleteAttribute(const string& name)
	{
		session.deleteAttribute(ip, name);
	}

	unordered_map<string, string> HTTPRequest::getCookies() const
	{
		unordered_map<string, string> result;
		
		try
		{
			const string& cookies = parser->getHeaders().at("Cookie");
			size_t offset = 0;

			while (true)
			{
				size_t findKey = cookies.find('=', offset);
				size_t findValue = cookies.find("; ", offset);
				string::const_iterator startKey = cookies.begin() + offset;
				string::const_iterator endKey = cookies.begin() + findKey;
				string::const_iterator startValue = endKey + 1;
				string::const_iterator endValue = findValue != string::npos ? (cookies.begin() + findValue) : (cookies.end());

				result.insert(make_pair(string(startKey, endKey), string(startValue, endValue)));

				if (findValue == string::npos)
				{
					break;
				}

				offset = findValue + 2;
			}
			
		}
		catch (const out_of_range&)
		{
			
		}

		return result;
	}

	void HTTPRequest::sendAssetFile(const string& filePath, HTTPResponse& response, const unordered_map<string_view, string>* const variables)
	{
		if (isWebFrameworkDynamicPages(filePath) && variables)
		{
			dynamicResources.sendDynamicFile(filePath, response, *variables);
		}
		else
		{
			staticResources.sendStaticFile(filePath, response);
		}
	}

	const string& HTTPRequest::getIpV4ClientAddress() const
	{
		return ip;
	}

	streams::IOSocketStream& operator >> (streams::IOSocketStream& stream, HTTPRequest& request)
	{
		string data;

		stream >> data;

		if (data.find("HTTP") == string::npos)
		{
			throw web::WebException();
		}

		request.parser = make_unique<web::HTTPParser>(data);

		return stream;
	}

	ostream& operator << (ostream& stream, const HTTPRequest& request)
	{
		const web::HTTPParser& parser = *request.parser.get();
		const unordered_map<string, string>& headers = parser.getHeaders();

		stream << parser.getMethod() << " " << parser.getParameters() << " " << parser.getHTTPVersion() << endl;

		for (const auto& [name, value] : headers)
		{
			stream << name << ": " << value << endl;
		}

		stream << endl << parser.getBody();

		return stream;
	}
}
