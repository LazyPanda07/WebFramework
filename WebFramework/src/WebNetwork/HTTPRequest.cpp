#include "HTTPRequest.h"

using namespace std;

namespace framework
{
	HTTPRequest::HTTPRequest(SessionsManager& session, const string& ip) :
		session(session),
		ip(ip)
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