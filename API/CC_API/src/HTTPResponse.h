#pragma once

#include "DLLHandler.h"

typedef void* HTTPResponse;

typedef enum
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
} ResponseCodes;

WebFrameworkException setBody(HTTPResponse implementation, const char* body);

// WebFrameworkException setJSONBody(HTTPResponse implementation, const char* body);

WebFrameworkException setHTTPVersion(HTTPResponse implementation, const char* version);

WebFrameworkException setHTTPResponseCode(HTTPResponse implementation, ResponseCodes responseCode);

WebFrameworkException addHTTPResponseHeader(HTTPResponse implementation, const char* name, const char* value);

WebFrameworkException appendHTTPResponseBody(HTTPResponse implementation, const char* body);

WebFrameworkException addHTTPResponseCookie(HTTPResponse implementation, const char* name, const char* value);

WebFrameworkException setHTTPResponseDefault(HTTPResponse implementation);

WebFrameworkException setHTTPResponseIsValid(HTTPResponse implementation, bool isValid);

inline WebFrameworkException setBody(HTTPResponse implementation, const char* body)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseBody, body, &exception);

	return exception;
}

inline WebFrameworkException setHTTPVersion(HTTPResponse implementation, const char* version)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPVersion)(void* implementation, const char* version, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPVersion, version, &exception);

	return exception;
}

inline WebFrameworkException setHTTPResponseCode(HTTPResponse implementation, ResponseCodes responseCode)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseCode)(void* implementation, int64_t code, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseCode, (int64_t)responseCode, &exception);

	return exception;
}

inline WebFrameworkException addHTTPResponseHeader(HTTPResponse implementation, const char* name, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void (*addHTTPResponseHeader)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addHTTPResponseHeader, name, value, &exception);

	return exception;
}

inline WebFrameworkException appendHTTPResponseBody(HTTPResponse implementation, const char* body)
{
	WebFrameworkException exception = NULL;

	typedef void (*appendHTTPResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendHTTPResponseBody, body, &exception);

	return exception;
}

inline WebFrameworkException addHTTPResponseCookie(HTTPResponse implementation, const char* name, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void (*addHTTPResponseCookie)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addHTTPResponseCookie, name, value, &exception);

	return exception;
}

inline WebFrameworkException setHTTPResponseDefault(HTTPResponse implementation)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseDefault)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseDefault, &exception);

	return exception;
}

WebFrameworkException setHTTPResponseIsValid(HTTPResponse implementation, bool isValid)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseDefault)(void* implementation, bool isValid, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseDefault, isValid, &exception);

	return exception;
}
