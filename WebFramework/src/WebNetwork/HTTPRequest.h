#pragma once

#include "HTTPParser.h"
#include "Managers/SessionsManager.h"
#include "Interfaces/ISendFile.h"
#include "BaseIOSocketStream.h"

namespace framework
{
	class HTTPRequest
	{
	private:
		std::unique_ptr<web::HTTPParser> parser;
		SessionsManager& session;
		const std::string ip;
		interfaces::ISendFile& resources;

	public:
		HTTPRequest(SessionsManager& session, const std::string& ip, interfaces::ISendFile& resources);

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

		//filePath must be with leading /
		void sendAssetFile(const std::string& filePath, HTTPResponse& response);

		friend streams::IOSocketStream& operator >> (streams::IOSocketStream& stream, HTTPRequest& request);

		friend std::ostream& operator << (std::ostream& stream, const HTTPRequest& request);

		~HTTPRequest() = default;
	};
}