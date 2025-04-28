#pragma once

#include "functional"

#include "WebNetwork/Interfaces/IHTTPResponse.h"
#include "JSONBuilder.h"
#include "ExecutorsConstants.h"

namespace framework
{
	class EXECUTORS_API HTTPResponse
	{
	private:
		interfaces::IHTTPResponse* implementation;
		std::function<void(interfaces::IHTTPResponse*)> deleter;

	public:
		HTTPResponse(interfaces::IHTTPResponse* implementation, const std::function<void(interfaces::IHTTPResponse*)>& deleter = nullptr);

		void setHTTPVersion(std::string_view version);

		/// <summary>
		/// Set HTTP response code
		/// </summary>
		/// <param name="code">value from web::responseCodes</param>
		void setResponseCode(web::ResponseCodes code);

		/// <summary>
		/// Set additional HTTP header
		/// </summary>
		/// <param name="name">of HTTP header</param>
		/// <param name="value">of HTTP header</param>
		void addHeader(std::string_view name, std::string_view value);

		/// <summary>
		/// <para>Some data with HTTP response</para>
		/// <para>Content-Length header setting automatically</para>
		/// </summary>
		/// <param name="body">data</param>
		void setBody(std::string_view body);

		/// <summary>
		/// <para>Some JSON data with HTTP response</para>
		/// <para>Content-Length and Content-Type headers setting automatically</para>
		/// </summary>
		/// <param name="json">JSON data</param>
		void setBody(const json::JSONBuilder& json);

		/// <summary>
		/// <para>Some data with HTTP response</para>
		/// <para>Content-Length header setting automatically</para>
		/// </summary>
		/// <param name="body">data</param>
		/// <returns>reference to self</returns>
		HTTPResponse& appendBody(std::string_view body);

		/// <summary>
		/// Add cookie to HTTP response
		/// </summary>
		/// <param name="name">of cookie</param>
		/// <param name="value">of cookie</param>
		void addCookie(std::string_view name, std::string_view value);

		/// <summary>
		/// Clears HTTPResponse
		/// </summary>
		void setDefault();

		void setIsValid(bool isValid);

		/**
		* isValid getter
		*/
		explicit operator bool() const;

		~HTTPResponse();

		friend class HTTPRequest;
	};
}
