#pragma once

#include "HttpResponse.h"
#include "Databases/Database.h"

typedef void* HTTPRequest;

/**
 * @brief URL encoded parameters
 */
typedef struct QueryParameter
{
	const char* key;
	const char* value;
} QueryParameter_t;

/**
 * @brief Single chunk in request with Transfer-Encoding: chunk header
 */
typedef struct HTTPChunk
{
	const char* data;
	size_t size;
} HTTPChunk_t;

/**
 * @brief HTTP header
 */
typedef struct HTTPHeader
{
	const char* key;
	const char* value;
} HTTPHeader_t;

/**
 * @brief HTTP cookie
 */
typedef struct Cookie
{
	const char* key;
	const char* value;
} Cookie_t;

/**
 * @brief Variable for processing .wfdp files
 */
typedef struct DynamicPagesVariable
{
	const char* name;
	const char* value;
} DynamicPagesVariable_t;

/**
 * @brief Data part if file passes large files threshold
 */
typedef struct LargeData
{
	const char* dataPart;
	size_t dataPartSize;
	bool isLastPacket;
} LargeData_t;

/**
 * @brief Mutilpart data from forms
 */
typedef struct Multipart
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
} Multipart_t;

/**
 * @brief Raw string with query parameters
 * @param implementation HTTPRequest instance
 * @param rawParameters Single string with query parameters
 * @return Error if occurred
 */
WebFrameworkException getHTTPRawParameters(HTTPRequest implementation, const char** rawParameters);

/**
 * @brief Get HTTP method
 * @param implementation HTTPRequest instance
 * @param method HTTP method
 * @return Error if occurred
 */
WebFrameworkException getHTTPMethod(HTTPRequest implementation, const char** method);

/**
 * @brief Get query parameters
 * @param implementation HTTPRequest instance
 * @param result Query parameters. Delete with free function
 * @param size Size of query parameters
 * @return Error if occurred
 */
WebFrameworkException getQueryParameters(HTTPRequest implementation, QueryParameter_t** result, size_t* size);

/**
 * @brief Get HTTP version
 * @param implementation HTTPRequest instance
 * @param version String representation of HTTP version. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
WebFrameworkException getHTTPVersion(HTTPRequest implementation, WebFrameworkString* version);

/**
 * @brief Get HTTP headers
 * @param implementation HTTPRequest instance
 * @param result HTTP headers. Delete with free function
 * @param size Size of HTTP headers
 * @return Error if occurred
 */
WebFrameworkException getHTTPHeaders(HTTPRequest implementation, HTTPHeader_t** result, size_t* size);

/**
 * @brief Get single HTTP header
 * @param implementation HTTPRequest instance
 * @param headerName HTTP header name
 * @param result HTTP header value
 * @return Error if occurred
 */
WebFrameworkException getHTTPHeader(HTTPRequest implementation, const char* headerName, const char** result);

/**
 * @brief Get body of HTTP request
 * @param implementation HTTPRequest instance
 * @param body HTTP body
 * @param bodySize HTTP body size
 * @return Error if occurred
 */
WebFrameworkException getHTTPBody(HTTPRequest implementation, const char** body, size_t* bodySize);

/**
 * @brief Set HTTP session attribute
 * @param implementation HTTPRequest instance
 * @param name Attribute name
 * @param value Attribute value
 * @return Error if occurred
 */
WebFrameworkException setHTTPAttribute(HTTPRequest implementation, const char* name, const char* value);

/**
 * @brief Get HTTP session attribute
 * @param implementation HTTPRequest instance
 * @param name Attribute name
 * @param result Attribute value. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
WebFrameworkException getHTTPAttribute(HTTPRequest implementation, const char* name, WebFrameworkString* result);

/**
 * @brief Delete session
 * @param implementation HTTPRequest instance 
 * @return Error if occurred
 */
WebFrameworkException deleteHTTPSession(HTTPRequest implementation);

/**
 * @brief Remove HTTP session attribute
 * @param implementation HTTPRequest instance
 * @param name Attribute name
 * @return Error if occurred
 */
WebFrameworkException removeHTTPAttribute(HTTPRequest implementation, const char* name);

/**
 * @brief Get cookies
 * @param implementation HTTPRequest instance
 * @param result Cookies. Delete with free function
 * @param size Size of Cookies
 * @return Error if occurred
 */
WebFrameworkException getCookies(HTTPRequest implementation, Cookie_t** result, size_t* size);

/**
 * @brief Get multiparts
 * @param implementation HTTPRequest instance 
 * @param result Multiparts. Delete with free function
 * @param size Size of multiparts
 * @return Error if occurred
 */
WebFrameworkException getMultiparts(HTTPRequest implementation, Multipart_t** result, size_t* size);

/**
 * @brief Get current large data
 * @param implementation HTTPRequest instance
 * @param result LargeData
 * @return Error if occurred
 */
WebFrameworkException getLargeData(HTTPRequest implementation, const LargeData_t** result);

/**
 * @brief Send file
 * @param implementation HTTPRequest instance
 * @param filePath Relative path to file from assets directory
 * @param response HTTPResponse instance
 * @param variables Variables for processing if file is .wfdp
 * @param variablesSize Size of variables
 * @param isBinary Is binary file
 * @param fileName Optional parameter. Custom name
 * @return Error if occurred
 */
WebFrameworkException sendAssetFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable_t* variables, size_t variablesSize, bool isBinary, const char* fileName);

/**
 * @brief Send static file
 * @param implementation HTTPRequest instance
 * @param filePath Relative path to file from assets directory
 * @param response HTTPResponse instance
 * @param isBinary Is binary file
 * @param fileName Optional parameter. Custom name
 * @return Error if occurred
 */
WebFrameworkException sendStaticFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, bool isBinary, const char* fileName);

/**
 * @brief Send .wfdp file
 * @param implementation HTTPRequest instance
 * @param filePath Relative path to file from assets directory
 * @param response HTTPResponse instance
 * @param variables Variables for processing .wfdp file
 * @param variablesSize Size of variables
 * @param isBinary Is binary file
 * @param fileName Optional parameter. Custom name
 * @return Error if occurred
 */
WebFrameworkException sendWFDPFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const DynamicPagesVariable_t* variables, size_t variablesSize, bool isBinary, const char* fileName);

/**
 * @brief Send file with specific chunk size
 * @param implementation HTTPRequest instance
 * @param filePath Relative path to file from assets directory
 * @param response HTTPResponse instance
 * @param fileName Name of file
 * @param chunkSize Chunk size
 * @return Error if occurred
 */
WebFrameworkException streamFile(HTTPRequest implementation, const char* filePath, HTTPResponse response, const char* fileName, size_t chunkSize);

/**
 * @brief Register function for processing .wfdp files
 * @param implementation HTTPRequest instance
 * @param functionName Function name
 * @param function Callback called in processing .wfdp files
 * @param deleter Deleter for values from function
 * @return 
 */
WebFrameworkException registerWFDPFunction(HTTPRequest implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result));

/**
 * @brief Unregister function for processing .wfdp files
 * @param implementation HTTPRequest instance
 * @param functionName Function name
 * @return Error if occurred
 */
WebFrameworkException unregisterWFDPFunction(HTTPRequest implementation, const char* functionName);

/**
 * @brief Check is WFDP function registered
 * @param implementation HTTPRequest instance
 * @param functionName Function name
 * @param result Is function registered
 * @return Error if occurred
 */
WebFrameworkException isWFDPFunctionRegistered(HTTPRequest implementation, const char* functionName, bool* result);

/**
 * @brief Get HTTP body as JSON
 * @param implementation HTTPRequest instance
 * @param parser JSON data
 * @return Error if occurred
 */
WebFrameworkException getHTTPRequestJSON(HTTPRequest implementation, JSONParser* parser);

/**
 * @brief Get HTTP chunks
 * @param implementation HTTPRequest instance
 * @param result Chunks
 * @param size Size of chunks
 * @return Error if occurred
 */
WebFrameworkException getHTTPChunks(HTTPRequest implementation, HTTPChunk_t** result, size_t* size);

/**
 * @brief Get file content
 * @param implementation HTTPRequest instance
 * @param filePath Path to asset file from assets folder
 * @param result File content. Delete with free function
 * @param size File content size
 * @return Error if occurred
 */
WebFrameworkException getFile(HTTPRequest implementation, const char* filePath, const char** result, size_t* size);

/**
 * @brief Process static files like .md
 * @param implementation HTTPRequest instance
 * @param fileData Static file content
 * @param size fileData size
 * @param fileExtension Supported processing extension
 * @param result Processed data
 * @param resultSize Processed data size
 * @return 
 */
WebFrameworkException processStaticFile(HTTPRequest implementation, const char* fileData, size_t size, const char* fileExtension, const char** result, size_t* resultSize);

/**
 * @brief Process .wfdp files
 * @param implementation HTTPRequest instance
 * @param fileData WFDP file content
 * @param variables Variables for processing .wfdp file
 * @param variablesSize Size of variables
 * @param result Processed data
 * @param resultSize Processed data size
 * @return 
 */
WebFrameworkException processWFDPFile(HTTPRequest implementation, const char* fileData, size_t size, const DynamicPagesVariable_t* variables, size_t variablesSize, const char** result, size_t* resultSize);

/**
 * @brief Get raw HTTP request
 * @param implementation HTTPRequest instance
 * @param rawRequest Raw HTTP request
 * @return Error if occurred
 */
WebFrameworkException getHTTPRawRequest(HTTPRequest implementation, const char** rawRequest);

/**
 * @brief Get client IP v4
 * @param implementation HTTPRequest instance
 * @param ip IP. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
WebFrameworkException getClientIpV4(HTTPRequest implementation, WebFrameworkString* ip);

/**
 * @brief Get server IP v4
 * @param implementation HTTPRequest instance
 * @param ip IP. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
WebFrameworkException getServerIpV4(HTTPRequest implementation, WebFrameworkString* ip);

/**
 * @brief Get client port
 * @param implementation HTTPRequest instance
 * @param port Client port
 * @return Error if occurred
 */
WebFrameworkException getClientPort(HTTPRequest implementation, uint16_t* port);

/**
 * @brief Get server port
 * @param implementation HTTPRequest instance
 * @param port Server port
 * @return Error if occurred
 */
WebFrameworkException getServerPort(HTTPRequest implementation, uint16_t* port);

/**
 * @brief Tries to get database or create if can't get
 * @param implementation HTTPRequest instance
 * @param databaseName Database name
 * @param result Database instance
 * @return Error if occurred
 */
WebFrameworkException getOrCreateDatabaseHTTPRequest(HTTPRequest implementation, const char* databaseName, Database* result);

/**
 * @brief Get database
 * @param implementation HTTPRequest instance
 * @param databaseName Database name
 * @param result Database instance
 * @return Error if occurred
 */
WebFrameworkException getDatabaseHTTPRequest(HTTPRequest implementation, const char* databaseName, Database* result);

/**
 * @brief Tries to get table from database or create if can't get
 * @param implementation HTTPRequest instance
 * @param databaseName Database name
 * @param tableName Name of table
 * @param createTableQuery Create table query if can't get table
 * @param result Table instance
 * @return Error if occurred
 */
WebFrameworkException getOrCreateTableHTTPRequest(HTTPRequest implementation, const char* databaseName, const char* tableName, const char* createTableQuery, Table* result);

/**
 * @brief Get table from database
 * @param implementation HTTPRequest instance
 * @param databaseName Database name
 * @param tableName Name of table
 * @param result Table instance
 * @return Error if occurred
 */
WebFrameworkException getTableHTTPRequest(HTTPRequest implementation, const char* databaseName, const char* tableName, Table* result);

/**
 * @brief Get route parameter as /page/{}
 * @param implementation HTTPRequest instance
 * @param routeParameterName Route parameter name
 * @param result Route parameter value
 * @return Error if occurred
 */
WebFrameworkException getRouteIntegerParameter(HTTPRequest implementation, const char* routeParameterName, int64_t* result);

/**
 * @brief Get route parameter as /page/{}
 * @param implementation HTTPRequest instance
 * @param routeParameterName Route parameter name
 * @param result Route parameter value
 * @return Error if occurred
 */
WebFrameworkException getRouteDoubleParameter(HTTPRequest implementation, const char* routeParameterName, double* result);

/**
 * @brief Get route parameter as /page/{}
 * @param implementation HTTPRequest instance
 * @param routeParameterName Route parameter name
 * @param result Route parameter value
 * @return Error if occurred
 */
WebFrameworkException getRouteStringParameter(HTTPRequest implementation, const char* routeParameterName, const char** result);

/**
 * @brief Send generated chunks
 * @param implementation HTTPRequest instance
 * @param response HTTPResponse instance
 * @param chunkGenerator Function that generates chunks
 * @param data Custom data
 * @return Error if occurred
 */
WebFrameworkException sendChunks(HTTPRequest implementation, HTTPResponse response, const char* (*chunkGenerator)(void* data), void* data);

/**
 * @brief Send file as chunks
 * @param implementation HTTPRequest instance
 * @param response HTTPResponse instance
 * @param fileName File name
 * @param chunkGenerator Function that generate chunks
 * @param data Custom data
 * @return Error if occurred
 */
WebFrameworkException sendFileChunks(HTTPRequest implementation, HTTPResponse response, const char* fileName, const char* (*chunkGenerator)(void* data), void* data);

/**
 * @brief Return from function with message and HTTP response code. Used for throwing errors
 * @param implementation HTTPRequest instance
 * @param exceptionMessage Error message
 * @param responseCode HTTP response code
 * @param logCategory Log category for logging
 * @param exceptionHash Same errors must have same hash
 * @return 
 */
WebFrameworkException throwWebFrameworkException(HTTPRequest implementation, const char* exceptionMessage, ResponseCodes_t responseCode, const char* logCategory, size_t exceptionHash);
