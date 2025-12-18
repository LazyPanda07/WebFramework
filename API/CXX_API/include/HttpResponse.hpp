#pragma once

#include <functional>

#include "WebInterfaces/IHTTPResponse.hpp"

#include "JsonBuilder.hpp"

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

	class HttpResponse
	{
	private:
		interfaces::IHTTPResponse* implementation;

	public:
		interfaces::IHTTPResponse* getImplementation() const;

	public:
		HttpResponse(interfaces::IHTTPResponse* implementation);

		HttpResponse(const HttpResponse&) = delete;

		HttpResponse& operator =(const HttpResponse&) = delete;

		HttpResponse(HttpResponse&& other) noexcept;

		HttpResponse& operator =(HttpResponse&& other) noexcept;

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
		/// <para>Some Json data with HTTP response</para>
		/// <para>Content-Length and Content-Type headers setting automatically</para>
		/// </summary>
		/// <param name="json">Json data</param>
		void setBody(const JsonBuilder& json);

		/// <summary>
		/// <para>Some data with HTTP response</para>
		/// <para>Content-Length header setting automatically</para>
		/// </summary>
		/// <param name="body">data</param>
		/// <returns>reference to self</returns>
		HttpResponse& appendBody(std::string_view body);

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

		~HttpResponse();

		friend class HttpRequest;
	};
}

namespace framework
{
	inline interfaces::IHTTPResponse* HttpResponse::getImplementation() const
	{
		return implementation;
	}

	inline HttpResponse::HttpResponse(interfaces::IHTTPResponse* implementation) :
		implementation(implementation)
	{

	}

	inline HttpResponse::HttpResponse(HttpResponse&& other) noexcept
	{
		(*this) = std::move(other);
	}

	inline HttpResponse& HttpResponse::operator =(HttpResponse&& other) noexcept
	{
		implementation = other.implementation;

		other.implementation = nullptr;

		return *this;
	}

	inline void HttpResponse::setHTTPVersion(std::string_view version)
	{
		implementation->setHTTPVersion(version.data());
	}

	inline void HttpResponse::setResponseCode(ResponseCodes code)
	{
		implementation->setResponseCode(static_cast<int>(code));
	}

	inline void HttpResponse::addHeader(std::string_view name, std::string_view value)
	{
		implementation->addHeader(name.data(), value.data());
	}

	inline void HttpResponse::setBody(std::string_view body)
	{
		implementation->setBody(body.data());
	}

	inline void HttpResponse::setBody(const JsonBuilder& json)
	{
		implementation->addHeader("Content-Type", "application/json");

		implementation->setBody(json.build().data());
	}

	inline HttpResponse& HttpResponse::appendBody(std::string_view body)
	{
		implementation->appendBody(body.data());

		return *this;
	}

	inline void HttpResponse::addCookie(std::string_view name, std::string_view value)
	{
		implementation->addCookie(name.data(), value.data());
	}

	inline void HttpResponse::setDefault()
	{
		implementation->setDefault();
	}

	inline void HttpResponse::setIsValid(bool isValid)
	{
		implementation->setIsValid(isValid);
	}

	inline HttpResponse::operator bool() const
	{
		return implementation->getIsValid();
	}

	inline HttpResponse::~HttpResponse()
	{
		implementation = nullptr;
	}
}
