#include "WebFrameworkExecutorsCAPI.h"

#include <Log.h>
#include <JsonBuilder.h>
#include <JsonParser.h>

#include "WebInterfaces/IHTTPRequest.h"
#include "Utility/JSONSettingsParser.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {} in {} function", "C_API", e.what(), __func__); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error(std::format("Something went wrong in file: {} on line: {}", __FILE__, __LINE__));

void setHTTPResponseBody(HTTPResponseObject response, const char* body, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setBody(body);
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

void setHTTPResponseJSONBody(HTTPResponseObject response, JSONBuilder builder, Exception* exception)
{
	try
	{
		framework::interfaces::IHTTPResponse* implementation = static_cast<framework::interfaces::IHTTPResponse*>(response);

		implementation->addHeader("Content-Type", "application/json");

		implementation->setBody(static_cast<json::JsonBuilder*>(builder)->build().data());
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

void setHTTPVersion(HTTPResponseObject response, const char* version, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setHTTPVersion(version);
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

void setHTTPResponseCode(HTTPResponseObject response, int64_t responseCode, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setResponseCode(responseCode);
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

void addHTTPResponseHeader(HTTPResponseObject response, const char* name, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->addHeader(name, value);
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

void appendHTTPResponseBody(HTTPResponseObject response, const char* body, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->appendBody(body);
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

void addHTTPResponseCookie(HTTPResponseObject response, const char* name, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->addCookie(name, value);
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

void setHTTPResponseDefault(HTTPResponseObject response, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setDefault();
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

void setHTTPResponseIsValid(HTTPResponseObject response, bool isValid, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPResponse*>(response)->setIsValid(isValid);
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

const char* getHTTPRawParameters(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getRawParameters();
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

const char* getHTTPMethod(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getMethod();
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

void* getHTTPVersion(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return new std::string(std::format("HTTP/{}", static_cast<framework::interfaces::IHTTPRequest*>(request)->getHTTPVersion()));
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

const char* getHTTPBody(HTTPRequestObject request, size_t* bodySize, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getBody(bodySize);
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

void setHTTPAttribute(HTTPRequestObject request, const char* name, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->setAttribute(name, value);
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

void* getHTTPAttribute(HTTPRequestObject request, const char* name, Exception* exception)
{
	try
	{
		framework::interfaces::IHTTPRequest* implementation = static_cast<framework::interfaces::IHTTPRequest*>(request);
		const char* temp = implementation->getAttribute(name);
		std::string* result = new std::string(temp);

		implementation->deleteAttribute(temp);

		return result;
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

void deleteHTTPSession(HTTPRequestObject request, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->deleteSession();
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

void removeHTTPAttribute(HTTPRequestObject request, const char* name, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->removeAttribute(name);
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

JSONParser getHTTPRequestJSON(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return createJSONParserFromString(static_cast<framework::interfaces::IHTTPRequest*>(request)->getJSON(), exception);
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

const char* getHTTPRawRequest(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getRawRequest();
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

void* getClientIpV4(HTTPRequestObject request, Exception* exception)
{
	try
	{
		framework::interfaces::IHTTPRequest* implementation = static_cast<framework::interfaces::IHTTPRequest*>(request);
		const char* temp = implementation->getClientIpV4();
		std::string* result = new std::string(temp);

		implementation->deleteClientIpV4(temp);

		return result;
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

void* getServerIpV4(HTTPRequestObject request, Exception* exception)
{
	try
	{
		framework::interfaces::IHTTPRequest* implementation = static_cast<framework::interfaces::IHTTPRequest*>(request);
		const char* temp = implementation->getServerIpV4();
		std::string* result = new std::string(temp);

		implementation->deleteClientIpV4(temp);

		return result;
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

uint16_t getClientPort(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getClientPort();
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

uint16_t getServerPort(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getServerPort();
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

void registerWFDPFunction(HTTPRequestObject request, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result), Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->registerWFDPFunction(functionName, function, deleter);
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

void registerWFDPFunctionClass(HTTPRequestObject request, const char* functionName, const char* apiType, void* functionClass, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->registerWFDPFunctionClass(functionName, apiType, functionClass);
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

void unregisterWFDPFunction(HTTPRequestObject request, const char* functionName, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->unregisterWFDPFunction(functionName);
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

bool isWFDPFunctionRegistered(HTTPRequestObject request, const char* functionName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->isWFDPFunctionRegistered(functionName);
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

void getQueryParameters(HTTPRequestObject request, void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->getQueryParameters(initQueryBuffer, addQueryParameter, buffer);
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

void getHTTPChunks(HTTPRequestObject request, void(*initChunkBuffer)(size_t size, void* buffer), void(*addChunk)(const char* chunk, size_t chunkSize, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->getChunks(initChunkBuffer, addChunk, buffer);
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

void getFile(HTTPRequestObject request, const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->getFile(filePath, fillBuffer, buffer);
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

void processStaticFile(HTTPRequestObject request, const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->processStaticFile(fileData, size, fileExtension, fillBuffer, buffer);
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

void processWFDPFile(HTTPRequestObject request, const char* fileData, size_t size, const DynamicPagesVariable variables, size_t variablesSize, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->processWFDPFile(fileData, size, static_cast<framework::interfaces::CVariable*>(variables), variablesSize, fillBuffer, buffer);
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

void getHTTPHeaders(HTTPRequestObject request, void(*initHeadersBuffer)(size_t size, void* buffer), void(*addHeader)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->getHeaders(initHeadersBuffer, addHeader, buffer);
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

const char* getHTTPHeader(HTTPRequestObject request, const char* headerName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getHeaderValue(headerName);
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

const void* getLargeData(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getLargeData();
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

void getMultiparts(HTTPRequestObject request, void(*initMultipartsBuffer)(size_t size, void* buffer), void(addMultipart)(const char* name, const char* fileName, const char* contentType, const char* data, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->getMultiparts(initMultipartsBuffer, addMultipart, buffer);
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

void getCookies(HTTPRequestObject request, void(*initCookiesBuffer)(size_t size, void* buffer), void(addCookie)(const char* key, const char* value, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->getCookies(initCookiesBuffer, addCookie, buffer);
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

void sendAssetFile(HTTPRequestObject request, const char* filePath, HTTPResponseObject response, const DynamicPagesVariable variables, size_t variableSize, bool isBinary, const char* fileName, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->sendAssetFile
		(
			filePath,
			static_cast<framework::interfaces::IHTTPResponse*>(response),
			variableSize,
			static_cast<framework::interfaces::CVariable*>(variables),
			isBinary,
			fileName
		);
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

void sendStaticFile(HTTPRequestObject request, const char* filePath, HTTPResponseObject response, bool isBinary, const char* fileName, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->sendStaticFile(filePath, static_cast<framework::interfaces::IHTTPResponse*>(response), isBinary, fileName);
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

void sendWFDPFile(HTTPRequestObject request, const char* filePath, HTTPResponseObject response, const DynamicPagesVariable variables, size_t variableSize, bool isBinary, const char* fileName, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->sendWFDPFile
		(
			filePath,
			static_cast<framework::interfaces::IHTTPResponse*>(response),
			variableSize,
			static_cast<framework::interfaces::CVariable*>(variables),
			isBinary,
			fileName
		);
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

void streamFile(HTTPRequestObject request, const char* filePath, HTTPResponseObject response, const char* fileName, size_t chunkSize, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::IHTTPRequest*>(request)->streamFile(filePath, static_cast<framework::interfaces::IHTTPResponse*>(response), fileName, chunkSize);
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

int64_t getRouteIntegerParameter(HTTPRequestObject request, const char* routeParameterName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getRouteIntegerParameter(routeParameterName);
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

double getRouteDoubleParameter(HTTPRequestObject request, const char* routeParameterName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getRouteDoubleParameter(routeParameterName);
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

const char* getRouteStringParameter(HTTPRequestObject request, const char* routeParameterName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getRouteStringParameter(routeParameterName);
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

DatabaseObject getOrCreateDatabaseHTTPRequest(HTTPRequestObject request, const char* databaseName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getOrCreateDatabase(databaseName);
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

DatabaseObject getDatabaseHTTPRequest(HTTPRequestObject request, const char* databaseName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getDatabase(databaseName);
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

TableObject getOrCreateTableHTTPRequest(HTTPRequestObject request, const char* databaseName, const char* tableName, const char* createTableQuery, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getOrCreateDatabase(databaseName)->getOrCreateTable(tableName, createTableQuery);
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

TableObject getTableHTTPRequest(HTTPRequestObject request, const char* databaseName, const char* tableName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->getDatabase(databaseName)->get(tableName);
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

void sendChunks(HTTPRequestObject request, HTTPResponseObject response, const char* (*chunkGenerator)(void* data), void* data, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->sendFileChunks(static_cast<framework::interfaces::IHTTPResponse*>(response), "", data, chunkGenerator);
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

void sendFileChunks(HTTPRequestObject request, HTTPResponseObject response, const char* fileName, const char* (*chunkGenerator)(void* data), void* data, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->sendFileChunks(static_cast<framework::interfaces::IHTTPResponse*>(response), fileName, data, chunkGenerator);
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

void throwWebFrameworkException(HTTPRequestObject request, const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionHash, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->throwException(errorMessage, responseCode, logCategory, exceptionHash);
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

void setExceptionData(HTTPRequestObject request, const char* errorMessage, int32_t responseCode, const char* logCategory)
{
	static_cast<framework::interfaces::IHTTPRequest*>(request)->setExceptionData(errorMessage, responseCode, logCategory);
}

bool isExceptionDataValid(HTTPRequestObject request, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IHTTPRequest*>(request)->isExceptionDataValid();
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

String getExecutorUserAgentFilter(ExecutorSettings executorsSettings, Exception* exception)
{
	try
	{
		return new std::string(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->userAgentFilter);
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

String getExecutorAPIType(ExecutorSettings executorsSettings, Exception* exception)
{
	try
	{
		return new std::string(static_cast<framework::utility::JSONSettingsParser::ExecutorSettings*>(executorsSettings)->apiType);
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
