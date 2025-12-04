#pragma once

#include "WebFrameworkUtilityCAPI.h"

typedef void* HTTPResponseObject;
typedef void* HTTPRequestObject;
typedef void* DynamicPagesVariable;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void setHTTPResponseBody(HTTPResponseObject response, const char* body, Exception* exception);

EXPORT void setHTTPResponseJSONBody(HTTPResponseObject response, JSONBuilder builder, Exception* exception);

EXPORT void setHTTPVersion(HTTPResponseObject response, const char* version, Exception* exception);

EXPORT void setHTTPResponseCode(HTTPResponseObject response, int64_t responseCode, Exception* exception);

EXPORT void addHTTPResponseHeader(HTTPResponseObject response, const char* name, const char* value, Exception* exception);

EXPORT void appendHTTPResponseBody(HTTPResponseObject response, const char* body, Exception* exception);

EXPORT void addHTTPResponseCookie(HTTPResponseObject response, const char* name, const char* value, Exception* exception);

EXPORT void setHTTPResponseDefault(HTTPResponseObject response, Exception* exception);

EXPORT void setHTTPResponseIsValid(HTTPResponseObject response, bool isValid, Exception* exception);

EXPORT const char* getHTTPRawParameters(HTTPRequestObject request, Exception* exception);

EXPORT const char* getHTTPMethod(HTTPRequestObject request, Exception* exception);

EXPORT void* getHTTPVersion(HTTPRequestObject request, Exception* exception);

EXPORT const char* getHTTPBody(HTTPRequestObject request, size_t* bodySize, Exception* exception);

EXPORT void setHTTPAttribute(HTTPRequestObject request, const char* name, const char* value, Exception* exception);

EXPORT void* getHTTPAttribute(HTTPRequestObject request, const char* name, Exception* exception);

EXPORT void deleteHTTPSession(HTTPRequestObject request, Exception* exception);

EXPORT void removeHTTPAttribute(HTTPRequestObject request, const char* name, Exception* exception);

EXPORT JSONParser getHTTPRequestJSON(HTTPRequestObject request, Exception* exception);

EXPORT const char* getHTTPRawRequest(HTTPRequestObject request, Exception* exception);

EXPORT void* getClientIpV4(HTTPRequestObject request, Exception* exception);

EXPORT void* getServerIpV4(HTTPRequestObject request, Exception* exception);

EXPORT uint16_t getClientPort(HTTPRequestObject request, Exception* exception);

EXPORT uint16_t getServerPort(HTTPRequestObject request, Exception* exception);

EXPORT void registerWFDPFunction(HTTPRequestObject request, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result), Exception* exception);

EXPORT void registerWFDPFunctionClass(HTTPRequestObject request, const char* functionName, const char* apiType, void* functionClass, Exception* exception);

EXPORT void unregisterWFDPFunction(HTTPRequestObject request, const char* functionName, Exception* exception);

EXPORT bool isWFDPFunctionRegistered(HTTPRequestObject request, const char* functionName, Exception* exception);

EXPORT void getQueryParameters(HTTPRequestObject request, void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT void getHTTPChunks(HTTPRequestObject request, void(*initChunkBuffer)(size_t size, void* buffer), void(*addChunk)(const char* chunk, size_t chunkSize, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT void getFile(HTTPRequestObject request, const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception);

EXPORT void processStaticFile(HTTPRequestObject request, const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception);

EXPORT void processWFDPFile(HTTPRequestObject request, const char* fileData, size_t size, const DynamicPagesVariable variables, size_t variablesSize, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception);

EXPORT void getHTTPHeaders(HTTPRequestObject request, void(*initHeadersBuffer)(size_t size, void* buffer), void(*addHeader)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT const char* getHTTPHeader(HTTPRequestObject request, const char* headerName, Exception* exception);

EXPORT const void* getLargeData(HTTPRequestObject request, Exception* exception);

EXPORT void getMultiparts(HTTPRequestObject request, void(*initMultipartsBuffer)(size_t size, void* buffer), void(addMultipart)(const char* name, const char* fileName, const char* contentType, const char* data, size_t dataSize, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT void getCookies(HTTPRequestObject request, void(*initCookiesBuffer)(size_t size, void* buffer), void(addCookie)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT void sendAssetFile(HTTPRequestObject request, const char* filePath, HTTPResponseObject response, const DynamicPagesVariable variables, size_t variableSize, bool isBinary, const char* fileName, Exception* exception);

EXPORT void sendStaticFile(HTTPRequestObject request, const char* filePath, HTTPResponseObject response, bool isBinary, const char* fileName, Exception* exception);

EXPORT void sendWFDPFile(HTTPRequestObject request, const char* filePath, HTTPResponseObject response, const DynamicPagesVariable variables, size_t variableSize, bool isBinary, const char* fileName, Exception* exception);

EXPORT void streamFile(HTTPRequestObject request, const char* filePath, HTTPResponseObject response, const char* fileName, size_t chunkSize, Exception* exception);

EXPORT int64_t getRouteIntegerParameter(HTTPRequestObject request, const char* routeParameterName, Exception* exception);

EXPORT double getRouteDoubleParameter(HTTPRequestObject request, const char* routeParameterName, Exception* exception);

EXPORT const char* getRouteStringParameter(HTTPRequestObject request, const char* routeParameterName, Exception* exception);

EXPORT DatabaseObject getOrCreateDatabaseHTTPRequest(HTTPRequestObject request, const char* databaseName, Exception* exception);

EXPORT DatabaseObject getDatabaseHTTPRequest(HTTPRequestObject request, const char* databaseName, Exception* exception);

EXPORT TableObject getOrCreateTableHTTPRequest(HTTPRequestObject request, const char* databaseName, const char* tableName, const char* createTableQuery, Exception* exception);

EXPORT TableObject getTableHTTPRequest(HTTPRequestObject request, const char* databaseName, const char* tableName, Exception* exception);

EXPORT void sendChunks(HTTPRequestObject request, HTTPResponseObject response, const char* (*chunkGenerator)(void* data, size_t* size), void* data, Exception* exception);

EXPORT void sendFileChunks(HTTPRequestObject request, HTTPResponseObject response, const char* fileName, const char* (*chunkGenerator)(void* data, size_t* size), void* data, Exception* exception);

EXPORT void throwWebFrameworkException(HTTPRequestObject request, const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionHash, Exception* exception);

EXPORT void setExceptionData(HTTPRequestObject request, const char* errorMessage, int32_t responseCode, const char* logCategory);

EXPORT bool isExceptionDataValid(HTTPRequestObject request, Exception* exception);

EXPORT String getExecutorInitParameters(ExecutorSettings executorsSettings, Exception* exception);

EXPORT String getExecutorName(ExecutorSettings executorsSettings, Exception* exception);

EXPORT String getExecutorUserAgentFilter(ExecutorSettings executorsSettings, Exception* exception);

EXPORT String getExecutorAPIType(ExecutorSettings executorsSettings, Exception* exception);

EXPORT int getExecutorLoadType(ExecutorSettings executorsSettings, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
