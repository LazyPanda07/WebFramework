#pragma once

#include "HTTPParser.h"
#include "Managers/SessionsManager.h"
#include "Interfaces/ISendFile.h"
#include "BaseIOSocketStream.h"

namespace framework
{
	/// <summary>
	/// <para>Parsing HTTP request</para>
	/// <para>Accessing to sessions</para>
	/// <para>Overriding input stream operator for simplify HTTP request initializing</para>
	/// </summary>
	class HTTPRequest
	{
	private:
		std::unique_ptr<web::HTTPParser> parser;
		SessionsManager& session;
		const std::string ip;
		interfaces::ISendFile& resources;

	public:
		/// <summary>
		/// Construct HTTPRequest
		/// </summary>
		/// <param name="session">from WebServer</param>
		/// <param name="ip">client's address</param>
		/// <param name="resources">ResourceExecutor</param>
		HTTPRequest(SessionsManager& session, const std::string& ip, interfaces::ISendFile& resources);

		/// <summary>
		/// Parameters string from HTTP
		/// </summary>
		/// <returns>HTPP parameters</returns>
		std::string getRawParameters() const;

		/// <summary>
		/// HTTP request method
		/// </summary>
		/// <returns>HTTP method</returns>
		std::string getMethod() const;

		/// <summary>
		/// GET parameters
		/// </summary>
		/// <returns>GET parameters as map</returns>
		const std::unordered_map<std::string, std::string>& getKeyValueParameters() const;

		/// <summary>
		/// HTTP version
		/// </summary>
		/// <returns>HTTP version</returns>
		const std::string& getHTTPVersion() const;

		/// <summary>
		/// All HTTP headers
		/// </summary>
		/// <returns>HTTP headers as map</returns>
		const std::unordered_map<std::string, std::string>& getHeaders() const;

		/// <summary>
		/// HTTP request body
		/// </summary>
		/// <returns>HTTP request body</returns>
		const std::string& getBody() const;

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name">attribute name</param>
		/// <param name="value">attribute value</param>
		void setAttribute(const std::string& name, const std::string& value);

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name">attribute name</param>
		/// <returns>attribute value</returns>
		std::string getAttribute(const std::string& name);

		/// <summary>
		/// Session wrapper
		/// </summary>
		void deleteSession();

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name"></param>
		void deleteAttribute(const std::string& name);

		/// <summary>
		/// Client's cookies
		/// </summary>
		/// <returns>HTTP cookies as map</returns>
		std::unordered_map<std::string, std::string> getCookies() const;

		/// <summary>
		/// ResourceExecutor wrapper
		/// </summary>
		/// <param name="filePath">must start with leading /</param>
		/// <param name="response">with file</param>
		void sendAssetFile(const std::string& filePath, HTTPResponse& response);

		/// <summary>
		/// Getter for ip
		/// </summary>
		/// <returns>client's address</returns>
		const std::string& getIpV4ClientAddress() const;

		/// <summary>
		/// Reading HTTP request from network
		/// </summary>
		/// <param name="stream">special class for taking data from network</param>
		/// <param name="request">class instance</param>
		/// <returns>self for builder pattern</returns>
		friend streams::IOSocketStream& operator >> (streams::IOSocketStream& stream, HTTPRequest& request);

		/// <summary>
		/// Logging operator
		/// </summary>
		/// <param name="stream">any output source</param>
		/// <param name="request">class instance</param>
		/// <returns>self for builder pattern</returns>
		friend std::ostream& operator << (std::ostream& stream, const HTTPRequest& request);

		~HTTPRequest() = default;
	};
}
