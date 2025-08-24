#pragma once

#include "JSONBuilder.h"

typedef void* HTTPResponse;

typedef enum ResponseCodes
{
	CONTINUE_CODE = 100,
	SWITCHING_PROTOCOLS,
	PROCESSING,
	OK = 200,
	CREATED,
	ACCEPTED,
	NON_AUTHORITATIVE_INFORMATION,
	NO_CONTENT,
	RESET_CONTENT,
	PARTIAL_CONTENT,
	MULTI_STATUS,
	ALREADY_REPORTED,
	IM_USED = 226,
	MULTIPLE_CHOICES = 300,
	MOVED_PERMANENTLY,
	FOUND,
	SEE_OTHER,
	NOT_MODIFIED,
	USE_PROXY,
	TEMPORARY_REDIRECT = 307,
	PERMANENT_REDIRECT,
	BAD_REQUEST = 400,
	UNAUTHORIZED,
	PAYMENT_REQUIRED,
	FORBIDDEN,
	NOT_FOUND,
	METHOD_NOT_ALLOWED,
	NOT_ACCEPTABLE,
	PROXY_AUTHENTICATION_REQUIRED,
	REQUEST_TIMEOUT,
	CONFLICT,
	GONE,
	LENGTH_REQUIRED,
	PRECONDITION_FAILED,
	PAYLOAD_TOO_LARGE,
	URI_TOO_LONG,
	UNSUPPORTED_MEDIA_TYPE,
	RANGE_NOT_SATISFIABLE,
	EXPECTATION_FAILED,
	IAM_A_TEAPOT,
	AUTHENTICATION_TIMEOUT,
	MISDIRECTED_REQUEST = 421,
	UNPROCESSABLE_ENTITY,
	LOCKED,
	FAILED_DEPENDENCY,
	UPGRADE_REQUIRED = 426,
	PRECONDITION_REQUIRED = 428,
	TOO_MANY_REQUESTS,
	REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
	RETRY_WITH = 449,
	UNAVAILABLE_FOR_LEGAL_REASONS = 451,
	CLIENT_CLOSED_REQUEST = 499,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED,
	BAD_GATEWAY,
	SERVICE_UNAVAILABLE,
	GATEWAY_TIMEOUT,
	HTTP_VERSION_NOT_SUPPORTED,
	VARIANT_ALSO_NEGOTIATES,
	INSUFFICIENT_STORAGE,
	LOOP_DETECTED,
	BANDWIDTH_LIMIT_EXCEEDED,
	NOT_EXTENDED,
	NETWORK_AUTHENTICATION_REQUIRED,
	UNKNOWN_ERROR = 520,
	WEB_SERVER_IS_DOWN,
	CONNECTION_TIMED_OUT,
	ORIGIN_IS_UNREACHABLE,
	A_TIMEOUT_OCCURRED,
	SSL_HANDSHAKE_FAILED,
	INVALID_SSL_CERTIFICATE
} ResponseCodes_t;

WebFrameworkException setBody(HTTPResponse implementation, const char* body);

WebFrameworkException setJSONBody(HTTPResponse implementation, JSONBuilder body);

WebFrameworkException setHTTPVersion(HTTPResponse implementation, const char* version);

WebFrameworkException setHTTPResponseCode(HTTPResponse implementation, ResponseCodes_t responseCode);

WebFrameworkException addHTTPResponseHeader(HTTPResponse implementation, const char* name, const char* value);

WebFrameworkException appendHTTPResponseBody(HTTPResponse implementation, const char* body);

WebFrameworkException addHTTPResponseCookie(HTTPResponse implementation, const char* name, const char* value);

WebFrameworkException setHTTPResponseDefault(HTTPResponse implementation);

WebFrameworkException setHTTPResponseIsValid(HTTPResponse implementation, bool isValid);
