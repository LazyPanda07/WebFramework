#pragma once

#include <functional>

#include "WebInterfaces/IHTTPResponse.h"

#include "JSONBuilder.hpp"

namespace framework
{
	enum class ResponseCodes
	{
		continueCode = 100,
		switchingProtocols,
		processing,
		ok = 200,
		created,
		accepted,
		nonAuthoritativeInformation,
		noContent,
		resetContent,
		partialContent,
		multiStatus,
		alreadyReported,
		IMUsed = 226,
		multipleChoices = 300,
		movedPermanently,
		found,
		seeOther,
		notModified,
		useProxy,
		temporaryRedirect = 307,
		permanentRedirect,
		badRequest = 400,
		unauthorized,
		paymentRequired,
		forbidden,
		notFound,
		methodNotAllowed,
		notAcceptable,
		proxyAuthenticationRequired,
		requestTimeout,
		conflict,
		gone,
		lengthRequired,
		preconditionFailed,
		payloadTooLarge,
		URITooLong,
		unsupportedMediaType,
		rangeNotSatisfiable,
		expectationFailed,
		iamATeapot,
		authenticationTimeout,
		misdirectedRequest = 421,
		unprocessableEntity,
		locked,
		failedDependency,
		upgradeRequired = 426,
		preconditionRequired = 428,
		tooManyRequests,
		requestHeaderFieldsTooLarge = 431,
		retryWith = 449,
		unavailableForLegalReasons = 451,
		clientClosedRequest = 499,
		internalServerError = 500,
		notImplemented,
		badGateway,
		serviceUnavailable,
		gatewayTimeout,
		HTTPVersionNotSupported,
		variantAlsoNegotiates,
		insufficientStorage,
		loopDetected,
		bandwidthLimitExceeded,
		notExtended,
		networkAuthenticationRequired,
		unknownError = 520,
		webServerIsDown,
		connectionTimedOut,
		originIsUnreachable,
		aTimeoutOccurred,
		SSLHandshakeFailed,
		invalidSSLCertificate
	};

	class HTTPResponse
	{
	private:
		interfaces::IHTTPResponse* implementation;
		std::function<void(interfaces::IHTTPResponse*)> deleter;

	public:
		interfaces::IHTTPResponse* getImplementation() const;

	public:
		HTTPResponse(interfaces::IHTTPResponse* implementation, const std::function<void(interfaces::IHTTPResponse*)>& deleter = nullptr);

		HTTPResponse(const HTTPResponse&) = delete;

		HTTPResponse& operator =(const HTTPResponse&) = delete;

		HTTPResponse(HTTPResponse&& other) noexcept;

		HTTPResponse& operator =(HTTPResponse&& other) noexcept;

		void setHTTPVersion(std::string_view version);

		/// <summary>
		/// Set HTTP response code
		/// </summary>
		/// <param name="code">value from web::responseCodes</param>
		void setResponseCode(ResponseCodes code);

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
		void setBody(const JSONBuilder& json);

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

namespace framework
{
	inline interfaces::IHTTPResponse* HTTPResponse::getImplementation() const
	{
		return implementation;
	}

	inline HTTPResponse::HTTPResponse(interfaces::IHTTPResponse* implementation, const std::function<void(interfaces::IHTTPResponse*)>& deleter) :
		implementation(implementation),
		deleter(deleter)
	{

	}

	inline HTTPResponse::HTTPResponse(HTTPResponse&& other) noexcept
	{
		(*this) = std::move(other);
	}

	inline HTTPResponse& HTTPResponse::operator =(HTTPResponse&& other) noexcept
	{
		implementation = other.implementation;
		deleter = other.deleter;

		other.implementation = nullptr;
		other.deleter = nullptr;

		return *this;
	}

	inline void HTTPResponse::setHTTPVersion(std::string_view version)
	{
		implementation->setHTTPVersion(version.data());
	}

	inline void HTTPResponse::setResponseCode(ResponseCodes code)
	{
		implementation->setResponseCode(static_cast<int>(code));
	}

	inline void HTTPResponse::addHeader(std::string_view name, std::string_view value)
	{
		implementation->addHeader(name.data(), value.data());
	}

	inline void HTTPResponse::setBody(std::string_view body)
	{
		implementation->setBody(body.data());
	}

	inline void HTTPResponse::setBody(const JSONBuilder& json)
	{
		implementation->addHeader("Content-Type", "application/json");

		implementation->setBody(json.build().data());
	}

	inline HTTPResponse& HTTPResponse::appendBody(std::string_view body)
	{
		implementation->appendBody(body.data());

		return *this;
	}

	inline void HTTPResponse::addCookie(std::string_view name, std::string_view value)
	{
		implementation->addCookie(name.data(), value.data());
	}

	inline void HTTPResponse::setDefault()
	{
		implementation->setDefault();
	}

	inline void HTTPResponse::setIsValid(bool isValid)
	{
		implementation->setIsValid(isValid);
	}

	inline HTTPResponse::operator bool() const
	{
		return implementation->getIsValid();
	}

	inline HTTPResponse::~HTTPResponse()
	{
		if (deleter && implementation)
		{
			deleter(implementation);

			implementation = nullptr;
		}
	}
}
