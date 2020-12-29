#pragma once

#include "HTTPBuilder.h"
#include "BaseIOSocketStream.h"

namespace framework
{
	/// <summary>
	/// <para>HTTPBuilder wrapper</para>
	/// <para>Overriding output stream operator for simplify HTTP response sending</para>
	/// </summary>
	class HTTPResponse
	{
	private:
		web::HTTPBuilder builder;
		std::string body;

	private:
		static std::string getFullDate();

	public:
		HTTPResponse();

		/// <summary>
		/// Assign operator for HTTPBuilder
		/// </summary>
		/// <param name="builder">your HTTPBuilder</param>
		/// <returns>self for builder pattern</returns>
		HTTPResponse& operator = (const web::HTTPBuilder& builder);

		/// <summary>
		/// <para>Set HTTP version</para>
		/// <para>Default is HTTP/1.1</para>
		/// </summary>
		/// <param name="version">full HTTP version like HTTP/1.1</param>
		void setHTTPVersion(const std::string& version);

		/// <summary>
		/// Set HTTP response code
		/// </summary>
		/// <param name="code">value from web::ResponseCodes</param>
		void setResponseCode(web::ResponseCodes code);

		/// <summary>
		/// Set additional HTTP header
		/// </summary>
		/// <param name="name">of HTTP header</param>
		/// <param name="value">of HTTP header</param>
		void addHeader(const std::string& name, const std::string& value);

		/// <summary>
		/// <para>Some data with HTTP response</para>
		/// <para>Content-Length header setting automatically</para>
		/// </summary>
		/// <param name="body">data</param>
		void addBody(const std::string& body);

		/// <summary>
		/// Add cookie to HTTP response
		/// </summary>
		/// <param name="name">of cookie</param>
		/// <param name="value">of cookie</param>
		void addCookie(const std::string& name, const std::string& value);

		/// <summary>
		/// Clears HTTPResponse
		/// </summary>
		void setDefault();

		/// <summary>
		/// Sending HTTPResponse to network
		/// </summary>
		/// <param name="stream">special class for sending data to network</param>
		/// <param name="response">class instance</param>
		/// <returns>self for builder pattern</returns>
		friend streams::IOSocketStream& operator << (streams::IOSocketStream& stream, HTTPResponse& response);

		~HTTPResponse() = default;
	};
}
