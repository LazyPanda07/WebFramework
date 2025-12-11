#pragma once

#include "json_builder_t.h"

typedef void* http_response_t;

/**
 * @brief HTTP response codes
 */
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

/**
 * @brief Set HTTP body
 * @param implementation http_response_t instance
 * @param body HTTP body
 * @return Error if occurred
 */
web_framework_exception_t setBody(http_response_t implementation, const char* body);

/**
 * @brief Set HTTP body
 * @param implementation http_response_t instance
 * @param body HTTP Json body
 * @return Error if occurred
 */
web_framework_exception_t setJsonBody(http_response_t implementation, json_builder_t body);

/**
 * @brief Set HTTP version
 * @param implementation http_response_t instance
 * @param version HTTP version
 * @return Error if occurred
 */
web_framework_exception_t setHTTPVersion(http_response_t implementation, const char* version);

/**
 * @brief Set HTTP response code
 * @param implementation http_response_t instance
 * @param responseCode HTTP response code
 * @return Error if occurred
 */
web_framework_exception_t setHTTPResponseCode(http_response_t implementation, ResponseCodes_t responseCode);

/**
 * @brief Add HTTP header
 * @param implementation http_response_t instance
 * @param name HTTP header name
 * @param value HTTP header value
 * @return Error if occurred
 */
web_framework_exception_t addHTTPResponseHeader(http_response_t implementation, const char* name, const char* value);

/**
 * @brief Append HTTP body
 * @param implementation http_response_t instance
 * @param body HTTP body
 * @return Error if occurred
 */
web_framework_exception_t appendHTTPResponseBody(http_response_t implementation, const char* body);

/**
 * @brief Add cookie
 * @param implementation http_response_t instance
 * @param name Cookie name
 * @param value Cookie value
 * @return Error if occurred
 */
web_framework_exception_t addHTTPResponseCookie(http_response_t implementation, const char* name, const char* value);

/**
 * @brief Set http_response_t to default state
 * @param implementation http_response_t instance
 * @return Error if occurred
 */
web_framework_exception_t setHTTPResponseDefault(http_response_t implementation);

/**
 * @brief Set state of http_response_t
 * @param implementation http_response_t instance
 * @param isValid http_response_t state
 * @return Error if occurred
 */
web_framework_exception_t setHTTPResponseIsValid(http_response_t implementation, bool isValid);
