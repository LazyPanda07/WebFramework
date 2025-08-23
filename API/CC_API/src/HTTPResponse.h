#pragma once

#include "JSONBuilder.h"

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

WebFrameworkException setJSONBody(HTTPResponse implementation, JSONBuilder body);

WebFrameworkException setHTTPVersion(HTTPResponse implementation, const char* version);

WebFrameworkException setHTTPResponseCode(HTTPResponse implementation, ResponseCodes responseCode);

WebFrameworkException addHTTPResponseHeader(HTTPResponse implementation, const char* name, const char* value);

WebFrameworkException appendHTTPResponseBody(HTTPResponse implementation, const char* body);

WebFrameworkException addHTTPResponseCookie(HTTPResponse implementation, const char* name, const char* value);

WebFrameworkException setHTTPResponseDefault(HTTPResponse implementation);

WebFrameworkException setHTTPResponseIsValid(HTTPResponse implementation, bool isValid);
