#pragma once

#include "HTTPParser.h"
#include "Managers/SessionsManager.h"

namespace framework
{
	class HTTPRequest
	{
	private:
		web::HTTPParser parser;
		SessionsManager& session;
		const std::string ip;

	public:
		HTTPRequest(web::HTTPParser&& parser, SessionsManager& session, const std::string& ip);

		std::string getRawParameters() const;

		std::string getMethod() const;

		const std::unordered_map<std::string, std::string>& getKeyValueParameters() const;

		const std::string& getHTTPVersion() const;

		const std::unordered_map<std::string, std::string>& getHeaders() const;

		const std::string& getBody() const;

		void setAttribute(const std::string& name, const std::string& value);

		std::string getAttribute(const std::string& name);

		void deleteSession();

		void deleteAttribute(const std::string& name);

		~HTTPRequest() = default;
	};
}