#pragma once

#include "HTTPResponse.h"
#include "Databases/Database.h"

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

typedef struct LargeData
{
	const char* dataPart;
	size_t dataPartSize;
	bool isLastPacket;
} LargeData_t;

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

WebFrameworkException getLargeData(HTTPRequest implementation, const LargeData_t** result);

WebFrameworkException sendAssetFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable* variables, size_t variablesSize, bool isBinary, const char* fileName);

WebFrameworkException sendStaticFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, bool isBinary, const char* fileName);

WebFrameworkException sendWFDPFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable* variables, size_t variablesSize, bool isBinary, const char* fileName);

WebFrameworkException streamFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const char* fileName, size_t chunkSize);

WebFrameworkException registerWFDPFunction(HTTPRequest implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result));

WebFrameworkException unregisterWFDPFunction(HTTPRequest implementation, const char* functionName);

WebFrameworkException isWFDPFunctionRegistered(HTTPRequest implementation, const char* functionName, bool* result);

WebFrameworkException getHTTPRequestJSON(HTTPRequest implementation, JSONParser* parser);

WebFrameworkException getHTTPChunks(HTTPRequest implementation, HTTPChunk** result, size_t* size);

WebFrameworkException getHTTPRawRequest(HTTPRequest implementation, const char** rawRequest);

WebFrameworkException getClientIpV4(HTTPRequest implementation, WebFrameworkString* ip);

WebFrameworkException getServerIpV4(HTTPRequest implementation, WebFrameworkString* ip);

WebFrameworkException getClientPort(HTTPRequest implementation, uint16_t* port);

WebFrameworkException getServerPort(HTTPRequest implementation, uint16_t* port);

WebFrameworkException getOrCreateDatabaseHTTPRequest(HTTPRequest implementation, const char* databaseName, Database* result);

WebFrameworkException getDatabaseHTTPRequest(HTTPRequest implementation, const char* databaseName, Database* result);

WebFrameworkException getOrCreateTableHTTPRequest(HTTPRequest implementation, const char* databaseName, const char* tableName, const char* createTableQuery, Table* result);

WebFrameworkException getTableHTTPRequest(HTTPRequest implementation, const char* databaseName, const char* tableName, Table* result);

WebFrameworkException getRouteIntegerParameter(HTTPRequest implementation, const char* routeParameterName, int64_t* result);

WebFrameworkException getRouteDoubleParameter(HTTPRequest implementation, const char* routeParameterName, double* result);

WebFrameworkException getRouteStringParameter(HTTPRequest implementation, const char* routeParameterName, const char** result);

WebFrameworkException sendChunks(HTTPRequest implementation, HTTPResponse response, const char* (*chunkGenerator)(void* data), void* data);

WebFrameworkException sendFileChunks(HTTPRequest implementation, HTTPResponse response, const char* fileName, const char* (*chunkGenerator)(void* data), void* data);

WebFrameworkException throwWebFrameworkException(HTTPRequest implementation, const char* exceptionMessage, ResponseCodes_t responseCode, const char* logCategory, size_t exceptionHash);
