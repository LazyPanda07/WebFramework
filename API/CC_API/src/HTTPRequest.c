#include "HTTPRequest.h"

static void __initQueryBuffer(size_t querySize, void* buffer)
{
	QueryParameter** temp = (QueryParameter**)buffer;

	*temp = (QueryParameter*)malloc((querySize + 1) * sizeof(QueryParameter)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for query parameter\n");

		exit(1);
	}

	memset(&(*temp)[querySize], 0, sizeof(QueryParameter)); // fill with zeros last element
}

static void __initChunksBuffer(size_t size, void* buffer)
{
	HTTPChunk** temp = (HTTPChunk**)buffer;

	*temp = (HTTPChunk*)malloc((size + 1) * sizeof(HTTPChunk)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for chunks\n");

		exit(2);
	}

	memset(&(*temp)[size], 0, sizeof(HTTPChunk)); // fill with zeros last element
}

static void __initHeadersBuffer(size_t size, void* buffer)
{
	HTTPHeader** temp = (HTTPHeader**)buffer;

	*temp = (HTTPHeader*)malloc((size + 1) * sizeof(HTTPHeader)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for HTTP headers\n");

		exit(3);
	}

	memset(&(*temp)[size], 0, sizeof(HTTPHeader)); // fill with zeros last element
}

static void __initMultipartsBuffer(size_t size, void* buffer)
{
	Multipart** temp = (Multipart**)buffer;

	*temp = (Multipart*)malloc((size + 1) * sizeof(Multipart)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for multiparts\n");

		exit(4);
	}

	memset(&(*temp)[size], 0, sizeof(Multipart)); // fill with zeros last element
}

static void __initCookiesBuffer(size_t size, void* buffer)
{
	Cookie** temp = (Cookie**)buffer;

	*temp = (Cookie*)malloc((size + 1) * sizeof(Cookie)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for cookies\n");

		exit(5);
	}

	memset(&(*temp)[size], 0, sizeof(Cookie)); // fill with zeros last element
}

static void __addQueryParameter(const char* key, const char* value, size_t index, void* buffer)
{
	QueryParameter* temp = *(QueryParameter**)buffer;

	temp[index].key = key;
	temp[index].value = value;
}

static void __addChunk(const char* chunk, size_t chunkSize, size_t index, void* buffer)
{
	HTTPChunk* temp = *(HTTPChunk**)buffer;

	temp[index].data = chunk;
	temp[index].size = chunkSize;
}

static void __addHeader(const char* key, const char* value, size_t index, void* buffer)
{
	HTTPHeader* temp = *(HTTPHeader**)buffer;

	temp[index].key = key;
	temp[index].value = value;
}

static void __addMultipart(const char* name, const char* fileName, const char* contentType, const char* data, size_t index, void* buffer)
{
	Multipart* temp = *(Multipart**)buffer;

	temp[index].name = name;
	temp[index].fileName = fileName;
	temp[index].contentType = contentType;
	temp[index].data = data;
}

static void __addCookie(const char* key, const char* value, size_t index, void* buffer)
{
	Cookie* temp = *(Cookie**)buffer;

	temp[index].key = key;
	temp[index].value = value;
}

WebFrameworkException getHTTPRawParameters(HTTPRequest implementation, const char** rawParameters)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPRawParameters)(void* implementation, void** exception);

	*rawParameters = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPRawParameters, &exception);

	return exception;
}

WebFrameworkException getHTTPMethod(HTTPRequest implementation, const char** method)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPMethod)(void* implementation, void** exception);

	*method = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPMethod, &exception);

	return exception;
}

WebFrameworkException getQueryParameters(HTTPRequest implementation, QueryParameter** result, size_t* size)
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

	memset(emptyBuffer, 0, sizeof(QueryParameter));

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

WebFrameworkException getHTTPVersion(HTTPRequest implementation, WebFrameworkString* version)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getHTTPVersion)(void* implementation, void** exception);

	*version = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPVersion, &exception);

	return exception;
}

WebFrameworkException getHTTPHeaders(HTTPRequest implementation, HTTPHeader** result, size_t* size)
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

	memset(emptyBuffer, 0, sizeof(HTTPHeader));

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

WebFrameworkException getHTTPHeader(HTTPRequest implementation, const char* headerName, const char** result)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPHeader)(void* implementation, const char* headerName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPHeader, headerName, &exception);

	return exception;
}

WebFrameworkException getHTTPBody(HTTPRequest implementation, const char** body)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPBody)(void* implementation, void** exception);

	*body = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPBody, &exception);

	return exception;
}

WebFrameworkException setHTTPAttribute(HTTPRequest implementation, const char* name, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPAttribute)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPAttribute, name, value, &exception);

	return exception;
}

WebFrameworkException getHTTPAttribute(HTTPRequest implementation, const char* name, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getHTTPAttribute)(void* implementation, const char* name, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPAttribute, name, &exception);

	return exception;
}

WebFrameworkException deleteHTTPSession(HTTPRequest implementation)
{
	WebFrameworkException exception = NULL;

	typedef void (*deleteHTTPSession)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(deleteHTTPSession, &exception);

	return exception;
}

WebFrameworkException removeHTTPAttribute(HTTPRequest implementation, const char* name)
{
	WebFrameworkException exception = NULL;

	typedef void (*removeHTTPAttribute)(void* implementation, const char* name, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(removeHTTPAttribute, name, &exception);

	return exception;
}

WebFrameworkException getCookies(HTTPRequest implementation, Cookie** result, size_t* size)
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

	memset(emptyBuffer, 0, sizeof(Cookie));

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

WebFrameworkException getMultiparts(HTTPRequest implementation, Multipart** result, size_t* size)
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

	memset(emptyBuffer, 0, sizeof(Multipart));

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

WebFrameworkException getLargeData(HTTPRequest implementation, const LargeData** result)
{
	WebFrameworkException exception = NULL;

	typedef const void* (*getLargeData)(void* implementation, void** exception);

	*result = (const LargeData*)CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getLargeData, &exception);

	return exception;
}

WebFrameworkException sendAssetFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable* variables, size_t variablesSize, bool isBinary, const char* fileName)
{
	WebFrameworkException exception = NULL;

	typedef void (*sendAssetFile)(void* implementation, const char* filePath, void* response, const void* variables, size_t variablesSize, bool isBinary, const char* fileName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendAssetFile, filePath, response, variables, variablesSize, isBinary, fileName, &exception);

	return exception;
}

WebFrameworkException sendStaticFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, bool isBinary, const char* fileName)
{
	WebFrameworkException exception = NULL;

	typedef void (*sendStaticFile)(void* implementation, const char* filePath, void* response, bool isBinary, const char* fileName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendStaticFile, filePath, response, isBinary, fileName, &exception);

	return exception;
}

WebFrameworkException sendDynamicFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable* variables, size_t variablesSize, bool isBinary, const char* fileName)
{
	WebFrameworkException exception = NULL;

	typedef void (*sendDynamicFile)(void* implementation, const char* filePath, void* response, const void* variables, size_t variablesSize, bool isBinary, const char* fileName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendDynamicFile, filePath, response, variables, variablesSize, isBinary, fileName, &exception);

	return exception;
}

WebFrameworkException streamFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const char* fileName, size_t chunkSize)
{
	WebFrameworkException exception = NULL;

	typedef void (*streamFile)(void* implementation, const char* filePath, void* response, const char* fileName, size_t chunkSize, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(streamFile, filePath, response, fileName, chunkSize, &exception);

	return exception;
}

WebFrameworkException registerDynamicFunction(HTTPRequest implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result))
{
	WebFrameworkException exception = NULL;

	typedef void (*registerDynamicFunction)(void* implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result), void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(registerDynamicFunction, functionName, function, deleter, &exception);

	return exception;
}

WebFrameworkException unregisterDynamicFunction(HTTPRequest implementation, const char* functionName)
{
	WebFrameworkException exception = NULL;

	typedef void (*unregisterDynamicFunction)(void* implementation, const char* functionName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(unregisterDynamicFunction, functionName, &exception);

	return exception;
}

WebFrameworkException isDynamicFunctionRegistered(HTTPRequest implementation, const char* functionName, bool* result)
{
	WebFrameworkException exception = NULL;

	typedef bool (*isDynamicFunctionRegistered)(void* implementation, const char* functionName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isDynamicFunctionRegistered, functionName, &exception);

	return exception;
}

WebFrameworkException getHTTPRequestJSON(HTTPRequest implementation, JSONParser* parser)
{
	WebFrameworkException exception = NULL;

	typedef void* (*removeHTTPAttribute)(void* implementation, void** exception);

	*parser = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(removeHTTPAttribute, &exception);

	return exception;
}

WebFrameworkException getHTTPChunks(HTTPRequest implementation, HTTPChunk** result, size_t* size)
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

	memset(emptyBuffer, 0, sizeof(HTTPChunk));

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

WebFrameworkException getHTTPRawRequest(HTTPRequest implementation, const char** rawRequest)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getHTTPRawRequest)(void* implementation, void** exception);

	*rawRequest = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHTTPRawRequest, &exception);

	return exception;
}

WebFrameworkException getClientIpV4(HTTPRequest implementation, WebFrameworkString* ip)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getClientIpV4)(void* implementation, void** exception);

	*ip = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getClientIpV4, &exception);

	return exception;
}

WebFrameworkException getServerIpV4(HTTPRequest implementation, WebFrameworkString* ip)
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

WebFrameworkException getOrCreateDatabaseHTTPRequest(HTTPRequest implementation, const char* databaseName, Database* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getOrCreateDatabaseHTTPRequest)(void* implementation, const char* databaseName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateDatabaseHTTPRequest, databaseName, &exception);

	return exception;
}

WebFrameworkException getDatabaseHTTPRequest(HTTPRequest implementation, const char* databaseName, Database* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getDatabaseHTTPRequest)(void* implementation, const char* databaseName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseHTTPRequest, databaseName, &exception);

	return exception;
}

WebFrameworkException getOrCreateTableHTTPRequest(HTTPRequest implementation, const char* databaseName, const char* tableName, const char* createTableQuery, Table* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getOrCreateTableHTTPRequest)(void* implementation, const char* databaseName, const char* tableName, const char* createTableQuery, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateTableHTTPRequest, databaseName, tableName, createTableQuery, &exception);

	return exception;
}

WebFrameworkException getTableHTTPRequest(HTTPRequest implementation, const char* databaseName, const char* tableName, Table* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getTableHTTPRequest)(void* implementation, const char* databaseName, const char* tableName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getTableHTTPRequest, databaseName, tableName, &exception);

	return exception;
}

WebFrameworkException getRouteIntegerParameter(HTTPRequest implementation, const char* routeParameterName, int64_t* result)
{
	WebFrameworkException exception = NULL;

	typedef int64_t(*getRouteIntegerParameter)(void* implementation, const char* routeParameterName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRouteIntegerParameter, routeParameterName, &exception);

	return exception;
}

WebFrameworkException getRouteDoubleParameter(HTTPRequest implementation, const char* routeParameterName, double* result)
{
	WebFrameworkException exception = NULL;

	typedef double(*getRouteDoubleParameter)(void* implementation, const char* routeParameterName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRouteDoubleParameter, routeParameterName, &exception);

	return exception;
}

WebFrameworkException getRouteStringParameter(HTTPRequest implementation, const char* routeParameterName, const char** result)
{
	WebFrameworkException exception = NULL;

	typedef const char* (*getRouteStringParameter)(void* implementation, const char* routeParameterName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRouteStringParameter, routeParameterName, &exception);

	return exception;
}

WebFrameworkException sendChunks(HTTPRequest implementation, HTTPResponse response, const char* (*chunkGenerator)(void* data), void* data)
{
	WebFrameworkException exception = NULL;

	typedef void (*sendChunks)(void* implementation, void* response, const char* (*chunkGenerator)(void* data), void* data, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendChunks, response, chunkGenerator, data, &exception);

	return exception;
}

WebFrameworkException sendFileChunks(HTTPRequest implementation, HTTPResponse response, const char* fileName, const char* (*chunkGenerator)(void* data), void* data)
{
	WebFrameworkException exception = NULL;

	typedef void (*sendFileChunks)(void* implementation, void* response, const char* fileName, const char* (*chunkGenerator)(void* data), void* data, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendFileChunks, response, fileName, chunkGenerator, data, &exception);

	return exception;
}

WebFrameworkException throwWebFrameworkException(HTTPRequest implementation, const char* errorMessage, ResponseCodes responseCode, const char* logCategory, size_t exceptionHash)
{
	WebFrameworkException exception = NULL;

	typedef void (*throwWebFrameworkException)(void* implementation, const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionHash, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(throwWebFrameworkException, errorMessage, (int64_t)responseCode, logCategory, exceptionHash, &exception);

	return exception;
}
