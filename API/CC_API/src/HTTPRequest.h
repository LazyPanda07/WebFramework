#pragma once

#include "HTTPResponse.h"

typedef void* HTTPRequest;

typedef struct
{
	const char* key;
	const char* value;
} QueryParameter;

void __initQueryBuffer(size_t querySize, void* buffer);

void __addQueryParameter(const char* key, const char* value, size_t index, void* buffer);

WebFrameworkException getHTTPRawParameters(HTTPRequest implementation, const char** rawParameters);

WebFrameworkException getHTTPMethod(HTTPRequest implementation, const char** method);

WebFrameworkException getQueryParameters(HTTPRequest implementation, QueryParameter** result);

WebFrameworkException getHTTPVersion(HTTPRequest implementation, WebFrameworkString* version);

// const HeadersMap& getHeaders() const;

WebFrameworkException getHTTPBody(HTTPRequest implementation, const char** body);

WebFrameworkException setHTTPAttribute(HTTPRequest implementation, const char* name, const char* value);

WebFrameworkException getHTTPAttribute(HTTPRequest implementation, const char* name, WebFrameworkString* result);

WebFrameworkException deleteHTTPSession(HTTPRequest implementation);

WebFrameworkException removeHTTPAttribute(HTTPRequest implementation, const char* name);

// HeadersMap getCookies() const;

// const std::vector<Multipart>& getMultiparts() const;

// LargeData getLargeData() const;

// void sendAssetFile(std::string_view filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables = {}, bool isBinary = true, std::string_view fileName = "");

// void sendStaticFile(std::string_view filePath, HTTPResponse& response, bool isBinary = true, std::string_view fileName = "");

// void sendDynamicFile(std::string_view filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary = false, std::string_view fileName = "");

// void streamFile(std::string_view filePath, HTTPResponse& response, std::string_view fileName, size_t chunkSize = interfaces::IHTTPRequest::defaultChunkSize);

// void registerDynamicFunction(std::string_view functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result));

// void unregisterDynamicFunction(std::string_view functionName);

bool isDynamicFunctionRegistered(HTTPRequest implementation, const char* functionName, bool* result);

WebFrameworkException getHTTPRequestJSON(HTTPRequest implementation, JSONParser* parser);

// const std::vector<std::string>& getChunks() const;

WebFrameworkException getHTTPRawRequest(HTTPRequest implementation, const char** rawRequest);

WebFrameworkException getClientIpV4(HTTPRequest implementation, WebFrameworkString* ip);

WebFrameworkException getServerIpV4(HTTPRequest implementation, WebFrameworkString* ip);

WebFrameworkException getClientPort(HTTPRequest implementation, uint16_t* port);

WebFrameworkException getServerPort(HTTPRequest implementation, uint16_t* port);

// Database getOrCreateDatabase(std::string_view databaseName);

// Database getDatabase(std::string_view databaseName) const;

// Table getOrCreateTable(std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery);

// Table getTable(std::string_view databaseName, std::string_view tableName) const;

// template<RouteParameterType T>
// T getRouteParameter(std::string_view routeParameterName) const;

// template<std::derived_from<utility::ChunkGenerator> T, typename... Args>
// void sendChunks(HTTPResponse& response, Args&&... args);

// template<std::derived_from<utility::ChunkGenerator> T, typename... Args>
// void sendFileChunks(HTTPResponse& response, std::string_view fileName, Args&&... args);

// template<std::derived_from<exceptions::WebFrameworkAPIException> T = exceptions::WebFrameworkAPIException, typename... Args>
// void throwException(Args&&... args);

inline void __initQueryBuffer(size_t querySize, void* buffer)
{
	QueryParameter** temp = (QueryParameter**)buffer;

	*temp = (QueryParameter*)malloc(querySize * sizeof(QueryParameter));
}

inline void __addQueryParameter(const char* key, const char* value, size_t index, void* buffer)
{
	QueryParameter* temp = *(QueryParameter**)buffer;

	temp[index].key = key;
	temp[index].value = value;
}

inline WebFrameworkException getHTTPRawParameters(HTTPRequest implementation, const char** rawParameters)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPRawParameters)(void* implementation, void** exception);

	*rawParameters = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPRawParameters, &exception);

	return exception;
}

inline WebFrameworkException getHTTPMethod(HTTPRequest implementation, const char** method)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPMethod)(void* implementation, void** exception);

	*method = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPMethod, &exception);

	return exception;
}

inline WebFrameworkException getQueryParameters(HTTPRequest implementation, QueryParameter** result)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getQueryParameters)(void* implementation, void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getQueryParameters, __initQueryBuffer, __addQueryParameter, result, &exception);

	return exception;
}

inline WebFrameworkException getHTTPVersion(HTTPRequest implementation, WebFrameworkString* version)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getHTTPVersion)(void* implementation, void** exception);

	*version = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPVersion, &exception);

	return exception;
}

inline WebFrameworkException getHTTPBody(HTTPRequest implementation, const char** body)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPBody)(void* implementation, void** exception);

	*body = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPBody, &exception);

	return exception;
}

inline WebFrameworkException setHTTPAttribute(HTTPRequest implementation, const char* name, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPAttribute)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPAttribute, name, value, &exception);

	return exception;
}

inline WebFrameworkException getHTTPAttribute(HTTPRequest implementation, const char* name, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getHTTPAttribute)(void* implementation, const char* name, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPAttribute, name, &exception);

	return exception;
}

inline WebFrameworkException deleteHTTPSession(HTTPRequest implementation)
{
	WebFrameworkException exception = NULL;

	typedef void (*deleteHTTPSession)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(deleteHTTPSession, &exception);

	return exception;
}

inline WebFrameworkException removeHTTPAttribute(HTTPRequest implementation, const char* name)
{
	WebFrameworkException exception = NULL;

	typedef void (*removeHTTPAttribute)(void* implementation, const char* name, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(removeHTTPAttribute, name, &exception);

	return exception;
}

inline bool isDynamicFunctionRegistered(HTTPRequest implementation, const char* functionName, bool* result)
{
	WebFrameworkException exception = NULL;

	typedef bool (*isDynamicFunctionRegistered)(void* implementation, const char* functionName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isDynamicFunctionRegistered, functionName, &exception);

	return exception;
}

inline WebFrameworkException getHTTPRequestJSON(HTTPRequest implementation, JSONParser* parser)
{
	WebFrameworkException exception = NULL;

	typedef void* (*removeHTTPAttribute)(void* implementation, void** exception);

	*parser = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(removeHTTPAttribute, &exception);

	return exception;
}

inline WebFrameworkException getHTTPRawRequest(HTTPRequest implementation, const char** rawRequest)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPRawRequest)(void* implementation, void** exception);

	*rawRequest = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPRawRequest, &exception);

	return exception;
}

inline WebFrameworkException getClientIpV4(HTTPRequest implementation, WebFrameworkString* ip)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getClientIpV4)(void* implementation, void** exception);

	*ip = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getClientIpV4, &exception);

	return exception;
}

inline WebFrameworkException getServerIpV4(HTTPRequest implementation, WebFrameworkString* ip)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getServerIpV4)(void* implementation, void** exception);

	*ip = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getServerIpV4, &exception);

	return exception;
}

WebFrameworkException getClientPort(HTTPRequest implementation, uint16_t* port)
{
	WebFrameworkException exception = NULL;

	typedef uint16_t(*getClientPort)(void* implementation, void** exception);

	*port = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getClientPort, &exception);

	return exception;
}

WebFrameworkException getServerPort(HTTPRequest implementation, uint16_t* port)
{
	WebFrameworkException exception = NULL;

	typedef uint16_t(*getServerPort)(void* implementation, void** exception);

	*port = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getServerPort, &exception);

	return exception;
}
