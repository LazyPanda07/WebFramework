#include "C_API/WebFrameworkExecutorsCApi.h"

#include <Log.h>
#include <JsonBuilder.h>
#include <JsonParser.h>
#include <jwt-cpp/jwt.h>

#include "WebInterfaces/IHttpRequest.h"
#include "Utility/JSONSettingsParser.h"
#include "Utility/ExecutorsUtility.h"
#include "Executors/ResourceExecutor.h"
#include "Databases/DatabaseImplementation.h"
#include "Managers/DatabasesManager.h"
#include "Web/HttpRequestImplementation.h"
#include "Exceptions/AlreadyLoggedException.h"
#include "Framework/WebFramework.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {} in {} function", "C_API", e.what(), __func__); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error(std::format("Something went wrong in file: {} on line: {}", __FILE__, __LINE__));

void setResponseBody(HttpResponseObject response, const char* body, size_t bodySize, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpResponse*>(response)->setBody(body, bodySize);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setResponseJsonBody(HttpResponseObject response, JsonBuilder builder, Exception* exception)
{
	try
	{
		framework::interfaces::IHttpResponse* implementation = static_cast<framework::interfaces::IHttpResponse*>(response);
		std::string jsonData = static_cast<json::JsonBuilder*>(builder)->build();

		implementation->addHeader("Content-Type", "application/json");

		implementation->setBody(jsonData.data(), jsonData.size());
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setVersion(HttpResponseObject response, const char* version, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpResponse*>(response)->setHTTPVersion(version);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setResponseCode(HttpResponseObject response, int64_t responseCode, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpResponse*>(response)->setResponseCode(responseCode);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void addResponseHeader(HttpResponseObject response, const char* name, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpResponse*>(response)->addHeader(name, value);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void appendResponseBody(HttpResponseObject response, const char* body, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpResponse*>(response)->appendBody(body);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void addResponseCookie(HttpResponseObject response, const char* name, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpResponse*>(response)->addCookie(name, value);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setResponseDefault(HttpResponseObject response, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpResponse*>(response)->setDefault();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setResponseIsValid(HttpResponseObject response, bool isValid, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpResponse*>(response)->setIsValid(isValid);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

const char* getRawParameters(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getRawParameters();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const char* getMethod(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getMethod();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void* getVersion(HttpRequestObject request, Exception* exception)
{
	try
	{
		return new std::string(std::format("HTTP/{}", static_cast<framework::interfaces::IHttpRequest*>(request)->getHTTPVersion()));
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const char* getBody(HttpRequestObject request, size_t* bodySize, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getBody(bodySize);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void setAttribute(HttpRequestObject request, const char* name, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->setAttribute(name, value);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void* getAttribute(HttpRequestObject request, const char* name, Exception* exception)
{
	try
	{
		framework::interfaces::IHttpRequest* implementation = static_cast<framework::interfaces::IHttpRequest*>(request);
		const char* temp = implementation->getAttribute(name);
		std::string* result = new std::string(temp);

		implementation->deleteAttribute(temp);

		return result;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void deleteSession(HttpRequestObject request, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->deleteSession();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void removeAttribute(HttpRequestObject request, const char* name, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->removeAttribute(name);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

JsonParser getRequestJson(HttpRequestObject request, Exception* exception)
{
	try
	{
		return createJsonParserFromString(static_cast<framework::interfaces::IHttpRequest*>(request)->getJson(), exception);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const char* getRawRequest(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getRawRequest();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void* getClientIpV4(HttpRequestObject request, Exception* exception)
{
	try
	{
		framework::interfaces::IHttpRequest* implementation = static_cast<framework::interfaces::IHttpRequest*>(request);
		const char* temp = implementation->getClientIpV4();
		std::string* result = new std::string(temp);

		implementation->deleteClientIpV4(temp);

		return result;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void* getServerIpV4(HttpRequestObject request, Exception* exception)
{
	try
	{
		framework::interfaces::IHttpRequest* implementation = static_cast<framework::interfaces::IHttpRequest*>(request);
		const char* temp = implementation->getServerIpV4();
		std::string* result = new std::string(temp);

		implementation->deleteClientIpV4(temp);

		return result;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

uint16_t getClientPort(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getClientPort();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}

uint16_t getServerPort(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getServerPort();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}

void registerDynamicFunctionClass(HttpRequestObject request, const char* functionName, const char* apiType, void* functionClass, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->registerDynamicFunctionClass(functionName, apiType, functionClass);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void unregisterDynamicFunction(HttpRequestObject request, const char* functionName, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->unregisterDynamicFunction(functionName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

bool isDynamicFunctionRegistered(HttpRequestObject request, const char* functionName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->isDynamicFunctionRegistered(functionName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

void getQueryParameters(HttpRequestObject request, void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->getQueryParameters(initQueryBuffer, addQueryParameter, buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void getChunks(HttpRequestObject request, void(*initChunkBuffer)(size_t size, void* buffer), void(*addChunk)(const char* chunk, size_t chunkSize, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->getChunks(initChunkBuffer, addChunk, buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void getFile(HttpRequestObject request, const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->getFile(filePath, fillBuffer, buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void processStaticFile(HttpRequestObject request, const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->processStaticFile(fileData, size, fileExtension, fillBuffer, buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void processDynamicFile(HttpRequestObject request, const char* fileData, size_t size, const void* arguments, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->processDynamicFile(fileData, size, arguments, fillBuffer, buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void getHeaders(HttpRequestObject request, void(*initHeadersBuffer)(size_t size, void* buffer), void(*addHeader)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->getHeaders(initHeadersBuffer, addHeader, buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

const char* getHeader(HttpRequestObject request, const char* headerName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getHeaderValue(headerName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const void* getLargeData(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getLargeData();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void getMultiparts(HttpRequestObject request, void(*initMultipartsBuffer)(size_t size, void* buffer), void(addMultipart)(const char* name, const char* fileName, const char* contentType, const char* data, size_t dataSize, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->getMultiparts(initMultipartsBuffer, addMultipart, buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void getCookies(HttpRequestObject request, void(*initCookiesBuffer)(size_t size, void* buffer), void(addCookie)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->getCookies(initCookiesBuffer, addCookie, buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void sendAssetFile(HttpRequestObject request, const char* filePath, HttpResponseObject response, const void* arguments, const char* fileName, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->sendAssetFile
		(
			filePath,
			static_cast<framework::interfaces::IHttpResponse*>(response),
			arguments,
			fileName
		);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void sendStaticFile(HttpRequestObject request, const char* filePath, HttpResponseObject response, const char* fileName, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->sendStaticFile
		(
			filePath, 
			static_cast<framework::interfaces::IHttpResponse*>(response), 
			fileName
		);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void sendDynamicFile(HttpRequestObject request, const char* filePath, HttpResponseObject response, const void* arguments, const char* fileName, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->sendDynamicFile
		(
			filePath,
			static_cast<framework::interfaces::IHttpResponse*>(response),
			arguments,
			fileName
		);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void streamFile(HttpRequestObject request, const char* filePath, HttpResponseObject response, const char* fileName, size_t chunkSize, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHttpRequest*>(request)->streamFile(filePath, static_cast<framework::interfaces::IHttpResponse*>(response), fileName, chunkSize);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

int64_t getRouteIntegerParameter(HttpRequestObject request, const char* routeParameterName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getRouteIntegerParameter(routeParameterName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}

double getRouteDoubleParameter(HttpRequestObject request, const char* routeParameterName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getRouteDoubleParameter(routeParameterName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0.0;
}

const char* getRouteStringParameter(HttpRequestObject request, const char* routeParameterName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getRouteStringParameter(routeParameterName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const char* getToken(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getToken();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

JsonObject getTokenPayload(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getTokenPayload();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

WebFramework getWebFrameworkInstance(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getWebFrameworkInstance();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

DatabaseObject getOrCreateDatabaseRequest(HttpRequestObject request, const char* databaseName, const char* implementationName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getOrCreateDatabase(databaseName, implementationName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

DatabaseObject getDatabaseRequest(HttpRequestObject request, const char* databaseName, const char* implementationName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getDatabase(databaseName, implementationName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

TableObject getOrCreateTableRequest(HttpRequestObject request, const char* databaseName, const char* implementationName, const char* tableName, const char* createTableQuery, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getOrCreateDatabase(databaseName, implementationName)->getOrCreateTable(tableName, createTableQuery);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

TableObject getTableRequest(HttpRequestObject request, const char* databaseName, const char* implementationName, const char* tableName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->getDatabase(databaseName, implementationName)->get(tableName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void sendChunks(HttpRequestObject request, HttpResponseObject response, const char* (*chunkGenerator)(void* data, size_t* size), void* data, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->sendFileChunks(static_cast<framework::interfaces::IHttpResponse*>(response), "", data, chunkGenerator);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void sendFileChunks(HttpRequestObject request, HttpResponseObject response, const char* fileName, const char* (*chunkGenerator)(void* data, size_t* size), void* data, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->sendFileChunks(static_cast<framework::interfaces::IHttpResponse*>(response), fileName, data, chunkGenerator);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void enqueueTask(HttpRequestObject request, const char* messageBrokerName, void* jsonObjectData, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->enqueueTask(messageBrokerName, jsonObjectData);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void throwWebFrameworkException(HttpRequestObject request, const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionHash, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->throwException(errorMessage, responseCode, logCategory, exceptionHash);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setExceptionData(HttpRequestObject request, const char* errorMessage, int32_t responseCode, const char* logCategory)
{
	static_cast<framework::interfaces::IHttpRequest*>(request)->setExceptionData(errorMessage, responseCode, logCategory);
}

bool isExceptionDataValid(HttpRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHttpRequest*>(request)->isExceptionDataValid();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

String getExecutorInitParameters(ExecutorSettings executorsSettings, Exception* exception)
{
	try
	{
		json::JsonParser parser(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->initParameters);
		std::ostringstream stream;

		stream << parser;

		return new std::string(stream.str());
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

String getExecutorName(ExecutorSettings executorsSettings, Exception* exception)
{
	try
	{
		return new std::string(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->name);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void getExecutorUserAgentFilter(ExecutorSettings executorsSettings, void(*initUserAgentFilterBuffer)(size_t size, void* buffer), void(*addUserAgentFilter)(const char* value, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		const std::vector<std::string>& userAgentFilter = static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->userAgentFilter;

		initUserAgentFilterBuffer(userAgentFilter.size(), buffer);

		for (size_t i = 0; i < userAgentFilter.size(); i++)
		{
			addUserAgentFilter(userAgentFilter[i].data(), i, buffer);
		}
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

String getExecutorAPIType(ExecutorSettings executorsSettings, Exception* exception)
{
	try
	{
		return new std::string(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->apiType);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

int getExecutorLoadType(ExecutorSettings executorsSettings, Exception* exception)
{
	try
	{
		return static_cast<int>(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->executorLoadType);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return -1;
}

DatabaseObject getOrCreateDatabaseExecutorSettings(ExecutorSettings executorsSettings, const char* databaseName, const char* implementationName, Exception* exception)
{
	try
	{
		framework::utility::JSONSettingsParser::ExecutorSettings& settings = *static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings);

		return settings.databases.emplace_back(new framework::DatabaseImplementation(settings.frameworkInstance.getDatabasesManager().getOrCreateDatabase(databaseName, implementationName)));
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

DatabaseObject getDatabaseExecutorSettings(ExecutorSettings executorsSettings, const char* databaseName, const char* implementationName, Exception* exception)
{
	try
	{
		framework::utility::JSONSettingsParser::ExecutorSettings& settings = *static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings);

		return settings.databases.emplace_back(new framework::DatabaseImplementation(settings.frameworkInstance.getDatabasesManager().getDatabase(databaseName, implementationName)));
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

TableObject getOrCreateTableExecutorSettings(ExecutorSettings executorsSettings, const char* databaseName, const char* implementationName, const char* tableName, const char* createTableQuery, Exception* exception)
{
	try
	{
		framework::utility::JSONSettingsParser::ExecutorSettings& settings = *static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings);

		return settings.databases.emplace_back(new framework::DatabaseImplementation(settings.frameworkInstance.getDatabasesManager().getOrCreateDatabase(databaseName, implementationName)))->getOrCreateTable(tableName, createTableQuery);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

TableObject getTableExecutorSettings(ExecutorSettings executorsSettings, const char* databaseName, const char* implementationName, const char* tableName, Exception* exception)
{
	try
	{
		framework::utility::JSONSettingsParser::ExecutorSettings& settings = *static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings);

		return settings.databases.emplace_back(new framework::DatabaseImplementation(settings.frameworkInstance.getDatabasesManager().getDatabase(databaseName, implementationName)))->get(tableName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void registerDynamicFunctionClassExecutorSettings(ExecutorSettings executorsSettings, const char* functionName, const char* apiType, void* functionClass, Exception* exception)
{
	try
	{
		framework::HttpRequestImplementation::registerDynamicFunctionClassStatic
		(
			functionName, apiType, functionClass,
			*std::static_pointer_cast<framework::ResourceExecutor>(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->resourceExecutor)
		);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void unregisterDynamicFunctionExecutorSettings(ExecutorSettings executorsSettings, const char* functionName, Exception* exception)
{
	try
	{
		std::static_pointer_cast<framework::ResourceExecutor>(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->resourceExecutor)->
			unregisterDynamicFunction(functionName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

bool isDynamicFunctionRegisteredExecutorSettings(ExecutorSettings executorsSettings, const char* functionName, Exception* exception)
{
	try
	{
		return std::static_pointer_cast<framework::ResourceExecutor>(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->resourceExecutor)->
			isDynamicFunctionRegistered(functionName);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

void getFileExecutorSettings(ExecutorSettings executorsSettings, const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		std::string data = std::static_pointer_cast<framework::ResourceExecutor>(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->resourceExecutor)->getFile(filePath);

		fillBuffer(data.data(), data.size(), buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void processStaticFileExecutorSettings(ExecutorSettings executorsSettings, const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		std::shared_ptr<framework::ResourceExecutor> resources = std::static_pointer_cast<framework::ResourceExecutor>(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->resourceExecutor);
		const std::unique_ptr<framework::interfaces::IStaticFileRenderer>& renderer = resources->getStaticRenderers().at(fileExtension);
		std::string result = renderer->render(std::string_view(fileData, size));

		fillBuffer(result.data(), result.size(), buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void processDynamicFileExecutorSettings(ExecutorSettings executorsSettings, const char* fileData, size_t size, const void* arguments, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		std::shared_ptr<framework::ResourceExecutor> resources = std::static_pointer_cast<framework::ResourceExecutor>(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->resourceExecutor);
		
		std::string result(fileData, size);

		resources->processDynamicFile(result, arguments);

		fillBuffer(result.data(), result.size(), buffer);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}
