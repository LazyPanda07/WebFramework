#pragma once

#include "core.h"

#include "HTTPBuilder.h"
#include "IOSocketStream.h"

namespace framework
{
	/// <summary>
	/// HTTPBuilder wrapper
	/// <para>Overriding output stream operator for simplify HTTP response sending</para>
	/// </summary>
	class WEB_FRAMEWORK_API HTTPResponse
	{
	private:
		web::HTTPBuilder builder;
		std::string body;
		bool isValid;

	public:
		/// <summary>
		/// Added full date GMT to response
		/// </summary>
		/// <returns>date in following format 'day,' 'day of the month' 'name of the month' 'full year' 'hours:minutes:seconds' GMT</returns>
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
		/// <param name="code">value from web::responseCodes</param>
		void setResponseCode(web::responseCodes code);

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
		/// <para>Some JSON data with HTTP response</para>
		/// <para>Content-Length and Content-Type headers setting automatically</para>
		/// </summary>
		/// <param name="json">JSON data</param>
		void addBody(const json::JSONBuilder& json);

		/// <summary>
		/// <para>Some data with HTTP response</para>
		/// <para>Content-Length header setting automatically</para>
		/// </summary>
		/// <param name="body">data</param>
		void addBody(std::string&& body) noexcept;

		/// <summary>
		/// <para>Some data with HTTP response</para>
		/// <para>Content-Length header setting automatically</para>
		/// </summary>
		/// <param name="body">data</param>
		/// <returns>reference to self</returns>
		HTTPResponse& appendBody(const std::string& body);

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

		void setIsValid(bool isValid);

		/**
		* isValid getter
		*/
		explicit operator bool() const;

		/// <summary>
		/// Sending HTTPResponse through network
		/// </summary>
		/// <param name="stream">special class for sending data through network</param>
		/// <param name="response">class instance</param>
		/// <returns>reference to self</returns>
		/// <exception cref="web::WebException"></exception>
		friend streams::IOSocketStream& operator << (streams::IOSocketStream& stream, HTTPResponse& response);

		~HTTPResponse() = default;
	};
}
