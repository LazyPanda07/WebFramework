#pragma once

#include "HTTPResponse.h"

typedef void* HTTPRequest;

typedef struct
{
	const char* key;
	const char* value;
} QueryParameter;

typedef struct
{
	const char* data;
	size_t size;
} HTTPChunk;

typedef struct
{
	const char* key;
	const char* value;
} HTTPHeader;

typedef struct
{
	const char* key;
	const char* value;
} Cookie;

typedef struct
{
	const char* name;
	const char* value;
} DynamicPagesVariable;

typedef struct
{
	const char* dataPart;
	size_t dataPartSize;
	size_t bodySize;
	bool isLastPacket;
} LargeData;

typedef struct
{
	const char* name;
	/**
	 * @brief May be NULL
	 */
	const char* fileName;
	/**
	 * @brief May be NULL
	 */
	const char* contentType;
	const char* data;
} Multipart;

void __initQueryBuffer(size_t querySize, void* buffer);

void __initChunksBuffer(size_t size, void* buffer);

void __initHeadersBuffer(size_t size, void* buffer);

void __initMultipartsBuffer(size_t size, void* buffer);

void __initCookiesBuffer(size_t size, void* buffer);

void __addQueryParameter(const char* key, const char* value, size_t index, void* buffer);

void __addChunk(const char* chunk, size_t chunkSize, size_t index, void* buffer);

void __addHeader(const char* key, const char* value, size_t index, void* buffer);

void __addMultipart(const char* name, const char* fileName, const char* contentType, const char* data, size_t index, void* buffer);

void __addCookie(const char* key, const char* value, size_t index, void* buffer);

WebFrameworkException getHTTPRawParameters(HTTPRequest implementation, const char** rawParameters);

WebFrameworkException getHTTPMethod(HTTPRequest implementation, const char** method);

WebFrameworkException getQueryParameters(HTTPRequest implementation, QueryParameter** result, size_t* size);

WebFrameworkException getHTTPVersion(HTTPRequest implementation, WebFrameworkString* version);

WebFrameworkException getHTTPHeaders(HTTPRequest implementation, HTTPHeader** result, size_t* size);

WebFrameworkException getHTTPHeader(HTTPRequest implementation, const char* headerName, const char** result);

WebFrameworkException getHTTPBody(HTTPRequest implementation, const char** body);

WebFrameworkException setHTTPAttribute(HTTPRequest implementation, const char* name, const char* value);

WebFrameworkException getHTTPAttribute(HTTPRequest implementation, const char* name, WebFrameworkString* result);

WebFrameworkException deleteHTTPSession(HTTPRequest implementation);

WebFrameworkException removeHTTPAttribute(HTTPRequest implementation, const char* name);

WebFrameworkException getCookies(HTTPRequest implementation, Cookie** result, size_t* size);

WebFrameworkException getMultiparts(HTTPRequest implementation, Multipart** result, size_t* size);

WebFrameworkException getLargeData(HTTPRequest implementation, const LargeData** result);

WebFrameworkException sendAssetFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable* variables, size_t variablesSize, bool isBinary, const char* fileName);

WebFrameworkException sendStaticFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, bool isBinary, const char* fileName);

WebFrameworkException sendDynamicFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable* variables, size_t variablesSize, bool isBinary, const char* fileName);

WebFrameworkException streamFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const char* fileName, size_t chunkSize);

WebFrameworkException registerDynamicFunction(HTTPRequest implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result));

WebFrameworkException unregisterDynamicFunction(HTTPRequest implementation, const char* functionName);

WebFrameworkException isDynamicFunctionRegistered(HTTPRequest implementation, const char* functionName, bool* result);

WebFrameworkException getHTTPRequestJSON(HTTPRequest implementation, JSONParser* parser);

WebFrameworkException getHTTPChunks(HTTPRequest implementation, HTTPChunk** result, size_t* size);

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

	*temp = (QueryParameter*)malloc((querySize + 1) * sizeof(QueryParameter)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for query parameter\n");

		exit(1);
	}

	memset(&(*temp)[querySize], NULL, sizeof(QueryParameter)); // fill with zeros last element
}

inline void __initChunksBuffer(size_t size, void* buffer)
{
	HTTPChunk** temp = (HTTPChunk**)buffer;

	*temp = (HTTPChunk*)malloc((size + 1) * sizeof(HTTPChunk)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for chunks\n");

		exit(2);
	}

	memset(&(*temp)[size], NULL, sizeof(HTTPChunk)); // fill with zeros last element
}

inline void __initHeadersBuffer(size_t size, void* buffer)
{
	HTTPHeader** temp = (HTTPHeader**)buffer;

	*temp = (HTTPHeader*)malloc((size + 1) * sizeof(HTTPHeader)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for HTTP headers\n");

		exit(3);
	}

	memset(&(*temp)[size], NULL, sizeof(HTTPHeader)); // fill with zeros last element
}

inline void __initMultipartsBuffer(size_t size, void* buffer)
{
	Multipart** temp = (Multipart**)buffer;

	*temp = (Multipart*)malloc((size + 1) * sizeof(Multipart)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for multiparts\n");

		exit(4);
	}

	memset(&(*temp)[size], NULL, sizeof(Multipart)); // fill with zeros last element
}

inline void __initCookiesBuffer(size_t size, void* buffer)
{
	Cookie** temp = (Cookie**)buffer;

	*temp = (Cookie*)malloc((size + 1) * sizeof(Cookie)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for multiparts\n");

		exit(5);
	}

	memset(&(*temp)[size], NULL, sizeof(Cookie)); // fill with zeros last element
}

inline void __addQueryParameter(const char* key, const char* value, size_t index, void* buffer)
{
	QueryParameter* temp = *(QueryParameter**)buffer;

	temp[index].key = key;
	temp[index].value = value;
}

inline void __addChunk(const char* chunk, size_t chunkSize, size_t index, void* buffer)
{
	HTTPChunk* temp = *(HTTPChunk**)buffer;

	temp[index].data = chunk;
	temp[index].size = chunkSize;
}

inline void __addHeader(const char* key, const char* value, size_t index, void* buffer)
{
	HTTPHeader* temp = *(HTTPHeader**)buffer;

	temp[index].key = key;
	temp[index].value = value;
}

inline void __addMultipart(const char* name, const char* fileName, const char* contentType, const char* data, size_t index, void* buffer)
{
	Multipart* temp = *(Multipart**)buffer;

	temp[index].name = name;
	temp[index].fileName = fileName;
	temp[index].contentType = contentType;
	temp[index].data = data;
}

inline void __addCookie(const char* key, const char* value, size_t index, void* buffer)
{
	Cookie* temp = *(Cookie**)buffer;

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

inline WebFrameworkException getQueryParameters(HTTPRequest implementation, QueryParameter** result, size_t* size)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getQueryParameters)(void* implementation, void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getQueryParameters, __initQueryBuffer, __addQueryParameter, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(QueryParameter)];
	QueryParameter* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, NULL, sizeof(QueryParameter));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(QueryParameter)))
		{
			*size = index;

			break;
		}

		index++;
	}

	return exception;
}

inline WebFrameworkException getHTTPVersion(HTTPRequest implementation, WebFrameworkString* version)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getHTTPVersion)(void* implementation, void** exception);

	*version = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPVersion, &exception);

	return exception;
}

inline WebFrameworkException getHTTPHeaders(HTTPRequest implementation, HTTPHeader** result, size_t* size)
{
	WebFrameworkException exception = NULL;

	typedef void (*getHTTPHeaders)(void* implementation, void(*initHeadersBuffer)(size_t size, void* buffer), void(*addHeader)(const char* key, const char* value, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPHeaders, __initHeadersBuffer, __addHeader, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(HTTPHeader)];
	HTTPHeader* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, NULL, sizeof(HTTPHeader));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(HTTPHeader)))
		{
			*size = index;

			break;
		}

		index++;
	}

	return exception;
}

inline WebFrameworkException getHTTPHeader(HTTPRequest implementation, const char* headerName, const char** result)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPHeader)(void* implementation, const char* headerName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPHeader, headerName, &exception);

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

inline WebFrameworkException getCookies(HTTPRequest implementation, Cookie** result, size_t* size)
{
	WebFrameworkException exception = NULL;

	typedef void (*getCookies)(void* implementation, void(*__initCookiesBuffer)(size_t size, void* buffer), void(*__addCookie)(const char* key, const char* value, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getCookies, __initCookiesBuffer, __addCookie, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(Cookie)];
	Cookie* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, NULL, sizeof(Cookie));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(Cookie)))
		{
			*size = index;

			break;
		}

		index++;
	}

	return exception;
}

inline WebFrameworkException getMultiparts(HTTPRequest implementation, Multipart** result, size_t* size)
{
	WebFrameworkException exception = NULL;

	typedef void (*getMultiparts)(void* implementation, void(*initMultipartsBuffer)(size_t size, void* buffer), void(*addMultipart)(const char* name, const char* fileName, const char* contentType, const char* data, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getMultiparts, __initMultipartsBuffer, __addMultipart, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(Multipart)];
	Multipart* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, NULL, sizeof(Multipart));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(Multipart)))
		{
			*size = index;

			break;
		}

		index++;
	}

	return exception;

	return exception;
}

inline WebFrameworkException getLargeData(HTTPRequest implementation, const LargeData** result)
{
	WebFrameworkException exception = NULL;

	typedef const void* (*getLargeData)(void* implementation, void** exception);

	*result = (const LargeData*)CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getLargeData, &exception);

	return exception;
}

inline WebFrameworkException sendAssetFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable* variables, size_t variablesSize, bool isBinary, const char* fileName)
{
	WebFrameworkException exception = NULL;

	typedef void (*sendAssetFile)(void* implementation, const char* filePath, void* response, const void* variables, size_t variablesSize, bool isBinary, const char* fileName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendAssetFile, filePath, response, variables, variablesSize, isBinary, fileName, &exception);

	return exception;
}

inline WebFrameworkException sendStaticFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, bool isBinary, const char* fileName)
{
	WebFrameworkException exception = NULL;

	typedef void (*sendStaticFile)(void* implementation, const char* filePath, void* response, bool isBinary, const char* fileName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendStaticFile, filePath, response, isBinary, fileName, &exception);

	return exception;
}

inline WebFrameworkException sendDynamicFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable* variables, size_t variablesSize, bool isBinary, const char* fileName)
{
	WebFrameworkException exception = NULL;

	typedef void (*sendDynamicFile)(void* implementation, const char* filePath, void* response, const void* variables, size_t variablesSize, bool isBinary, const char* fileName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendDynamicFile, filePath, response, variables, variablesSize, isBinary, fileName, &exception);

	return exception;
}

inline WebFrameworkException streamFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const char* fileName, size_t chunkSize)
{
	WebFrameworkException exception = NULL;

	typedef void (*streamFile)(void* implementation, const char* filePath, void* response, const char* fileName, size_t chunkSize, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(streamFile, filePath, response, fileName, chunkSize, &exception);

	return exception;
}

inline WebFrameworkException registerDynamicFunction(HTTPRequest implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result))
{
	WebFrameworkException exception = NULL;

	typedef void (*registerDynamicFunction)(void* implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result), void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(registerDynamicFunction, functionName, function, deleter, &exception);

	return exception;
}

inline WebFrameworkException unregisterDynamicFunction(HTTPRequest implementation, const char* functionName)
{
	WebFrameworkException exception = NULL;

	typedef void (*unregisterDynamicFunction)(void* implementation, const char* functionName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(unregisterDynamicFunction, functionName, &exception);

	return exception;
}

inline WebFrameworkException isDynamicFunctionRegistered(HTTPRequest implementation, const char* functionName, bool* result)
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

inline WebFrameworkException getHTTPChunks(HTTPRequest implementation, HTTPChunk** result, size_t* size)
{
	WebFrameworkException exception = NULL;

	typedef void (*getHTTPChunks)(void* implementation, void(*initChunkBuffer)(size_t size, void* buffer), void(*addChunk)(const char* chunk, size_t chunkSize, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPChunks, __initChunksBuffer, __addChunk, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(HTTPChunk)];
	HTTPChunk* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, NULL, sizeof(HTTPChunk));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(HTTPChunk)))
		{
			*size = index;

			break;
		}

		index++;
	}

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
