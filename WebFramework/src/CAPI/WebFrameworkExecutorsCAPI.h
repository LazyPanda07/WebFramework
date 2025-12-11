#pragma once

#include "WebFrameworkUtilityCAPI.h"

typedef void* HttpResponseObject;
typedef void* HttpRequestObject;
typedef void* DynamicPagesVariable;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void setResponseBody(HttpResponseObject response, const char* body, Exception* exception);

EXPORT void setResponseJsonBody(HttpResponseObject response, JsonBuilder builder, Exception* exception);

EXPORT void setVersion(HttpResponseObject response, const char* version, Exception* exception);

EXPORT void setResponseCode(HttpResponseObject response, int64_t responseCode, Exception* exception);

EXPORT void addResponseHeader(HttpResponseObject response, const char* name, const char* value, Exception* exception);

EXPORT void appendResponseBody(HttpResponseObject response, const char* body, Exception* exception);

EXPORT void addResponseCookie(HttpResponseObject response, const char* name, const char* value, Exception* exception);

EXPORT void setResponseDefault(HttpResponseObject response, Exception* exception);

EXPORT void setResponseIsValid(HttpResponseObject response, bool isValid, Exception* exception);

EXPORT const char* getRawParameters(HttpRequestObject request, Exception* exception);

EXPORT const char* getMethod(HttpRequestObject request, Exception* exception);

EXPORT void* getVersion(HttpRequestObject request, Exception* exception);

EXPORT const char* getBody(HttpRequestObject request, size_t* bodySize, Exception* exception);

EXPORT void setAttribute(HttpRequestObject request, const char* name, const char* value, Exception* exception);

EXPORT void* getAttribute(HttpRequestObject request, const char* name, Exception* exception);

EXPORT void deleteSession(HttpRequestObject request, Exception* exception);

EXPORT void removeAttribute(HttpRequestObject request, const char* name, Exception* exception);

EXPORT JsonParser getRequestJson(HttpRequestObject request, Exception* exception);

EXPORT const char* getRawRequest(HttpRequestObject request, Exception* exception);

EXPORT void* getClientIpV4(HttpRequestObject request, Exception* exception);

EXPORT void* getServerIpV4(HttpRequestObject request, Exception* exception);

EXPORT uint16_t getClientPort(HttpRequestObject request, Exception* exception);

EXPORT uint16_t getServerPort(HttpRequestObject request, Exception* exception);

EXPORT void registerWFDPFunction(HttpRequestObject request, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result), Exception* exception);

EXPORT void registerWFDPFunctionClass(HttpRequestObject request, const char* functionName, const char* apiType, void* functionClass, Exception* exception);

EXPORT void unregisterWFDPFunction(HttpRequestObject request, const char* functionName, Exception* exception);

EXPORT bool isWFDPFunctionRegistered(HttpRequestObject request, const char* functionName, Exception* exception);

EXPORT void getQueryParameters(HttpRequestObject request, void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT void getChunks(HttpRequestObject request, void(*initChunkBuffer)(size_t size, void* buffer), void(*addChunk)(const char* chunk, size_t chunkSize, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT void getFile(HttpRequestObject request, const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception);

EXPORT void processStaticFile(HttpRequestObject request, const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception);

EXPORT void processWFDPFile(HttpRequestObject request, const char* fileData, size_t size, const DynamicPagesVariable variables, size_t variablesSize, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception);

EXPORT void getHeaders(HttpRequestObject request, void(*initHeadersBuffer)(size_t size, void* buffer), void(*addHeader)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT const char* getHeader(HttpRequestObject request, const char* headerName, Exception* exception);

EXPORT const void* getLargeData(HttpRequestObject request, Exception* exception);

EXPORT void getMultiparts(HttpRequestObject request, void(*initMultipartsBuffer)(size_t size, void* buffer), void(addMultipart)(const char* name, const char* fileName, const char* contentType, const char* data, size_t dataSize, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT void getCookies(HttpRequestObject request, void(*initCookiesBuffer)(size_t size, void* buffer), void(addCookie)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception);

EXPORT void sendAssetFile(HttpRequestObject request, const char* filePath, HttpResponseObject response, const DynamicPagesVariable variables, size_t variableSize, bool isBinary, const char* fileName, Exception* exception);

EXPORT void sendStaticFile(HttpRequestObject request, const char* filePath, HttpResponseObject response, bool isBinary, const char* fileName, Exception* exception);

EXPORT void sendWFDPFile(HttpRequestObject request, const char* filePath, HttpResponseObject response, const DynamicPagesVariable variables, size_t variableSize, bool isBinary, const char* fileName, Exception* exception);

EXPORT void streamFile(HttpRequestObject request, const char* filePath, HttpResponseObject response, const char* fileName, size_t chunkSize, Exception* exception);

EXPORT int64_t getRouteIntegerParameter(HttpRequestObject request, const char* routeParameterName, Exception* exception);

EXPORT double getRouteDoubleParameter(HttpRequestObject request, const char* routeParameterName, Exception* exception);

EXPORT const char* getRouteStringParameter(HttpRequestObject request, const char* routeParameterName, Exception* exception);

EXPORT DatabaseObject getOrCreateDatabaseRequest(HttpRequestObject request, const char* databaseName, Exception* exception);

EXPORT DatabaseObject getDatabaseRequest(HttpRequestObject request, const char* databaseName, Exception* exception);

EXPORT TableObject getOrCreateTableRequest(HttpRequestObject request, const char* databaseName, const char* tableName, const char* createTableQuery, Exception* exception);

EXPORT TableObject getTableRequest(HttpRequestObject request, const char* databaseName, const char* tableName, Exception* exception);

EXPORT void sendChunks(HttpRequestObject request, HttpResponseObject response, const char* (*chunkGenerator)(void* data, size_t* size), void* data, Exception* exception);

EXPORT void sendFileChunks(HttpRequestObject request, HttpResponseObject response, const char* fileName, const char* (*chunkGenerator)(void* data, size_t* size), void* data, Exception* exception);

EXPORT void throwWebFrameworkException(HttpRequestObject request, const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionHash, Exception* exception);

EXPORT void setExceptionData(HttpRequestObject request, const char* errorMessage, int32_t responseCode, const char* logCategory);

EXPORT bool isExceptionDataValid(HttpRequestObject request, Exception* exception);

EXPORT String getExecutorInitParameters(ExecutorSettings executorsSettings, Exception* exception);

EXPORT String getExecutorName(ExecutorSettings executorsSettings, Exception* exception);

EXPORT String getExecutorUserAgentFilter(ExecutorSettings executorsSettings, Exception* exception);

EXPORT String getExecutorAPIType(ExecutorSettings executorsSettings, Exception* exception);

EXPORT int getExecutorLoadType(ExecutorSettings executorsSettings, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
