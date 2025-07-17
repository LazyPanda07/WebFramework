#pragma once

#include "IHTTPResponse.h"

#include "Databases/Interfaces/IDatabase.h"

namespace framework::interfaces
{
	struct WEB_FRAMEWORK_CORE_API CLargeData
	{
		const char* dataPart;
		size_t dataPartSize;
		size_t bodySize;
		bool isLastPacket;

		CLargeData() :
			dataPart(nullptr),
			dataPartSize(0),
			bodySize(0),
			isLastPacket(true)
		{

		}
	};

	struct WEB_FRAMEWORK_CORE_API CMultipart
	{
		const char* name;
		/**
		 * @brief May be nullptr
		 */
		const char* fileName;
		/**
		 * @brief May be nullptr
		 */
		const char* contentType;
		const char* data;

		CMultipart() :
			name(nullptr),
			fileName(nullptr),
			contentType(nullptr),
			data(nullptr)
		{

		}
	};

	struct WEB_FRAMEWORK_CORE_API CVariable
	{
		const char* name;
		const char* value;

		CVariable(const char* name = nullptr, const char* value = nullptr) :
			name(name),
			value(value)
		{

		}
	};

	class WEB_FRAMEWORK_CORE_API IHTTPRequest
	{
	public:
		IHTTPRequest() = default;

		virtual void updateLargeData(const char* dataPart, size_t dataPartSize, size_t bodySize) = 0;

		virtual const char* getRawParameters() const = 0;

		virtual const char* getMethod() const = 0;

		virtual void getKeyValueParameters(void(*addKeyValue)(const char* key, const char* value, void* additionalData), void* additionalData) const = 0;

		virtual const char* getKeyValueParameter(const char* key) const = 0;

		virtual double getHTTPVersion() const = 0;

		virtual void getHeaders(void(*addHeader)(const char* key, const char* value, void* additionalData), void* additionalData) const = 0;

		virtual const char* getHeaderValue(const char* headerName, bool throwException = true) const = 0;

		virtual const char* getBody() const = 0;

		virtual void setAttribute(const char* name, const char* value) = 0;

		virtual const char* getAttribute(const char* name) = 0;

		virtual void deleteAttribute(const char* attribute) = 0;

		virtual void deleteSession() = 0;

		virtual void removeAttribute(const char* name) = 0;

		virtual void getCookies(void(*addCookie)(const char* key, const char* value, void* additionalData), void* additionalData) const = 0;

		virtual void getMultiparts(void(*addMultipart)(const CMultipart* part, void* additionalData), void* additionalData) const = 0;

		virtual const CLargeData* getLargeData() const = 0;

		virtual void sendAssetFile(const char* filePath, IHTTPResponse* response, size_t variablesSize = 0, const CVariable* variables = nullptr, bool isBinary = true, const char* fileName = "") = 0;

		virtual void sendStaticFile(const char* filePath, IHTTPResponse* response, bool isBinary = true, const char* fileName = "") = 0;

		virtual void sendDynamicFile(const char* filePath, IHTTPResponse* response, size_t variablesSize, const CVariable* variables, bool isBinary = false, const char* fileName = "") = 0;

		virtual void streamFile(const char* filePath, IHTTPResponse* response, const char* fileName, size_t chunkSize = defaultChunkSize) = 0;

		virtual void registerDynamicFunction(const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*resultDeleter)(const char* result)) = 0;

		virtual void unregisterDynamicFunction(const char* functionName) = 0;

		virtual bool isDynamicFunctionRegistered(const char* functionName) = 0;

		virtual void sendFileChunks(IHTTPResponse* response, const char* fileName, void* chunkGenerator, const char* (*addChunk)(void* chunkGenerator)) = 0;

		virtual void throwException(const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionClassHash) = 0;

		virtual void getChunks(void(*addChunk)(const char* chunk, size_t chunkSize, void* additionalData), void* additionalData) const = 0;

		virtual const char* getJSON() const = 0;

		virtual const char* getRawRequest() const = 0;

		virtual const char* getClientIpV4() const = 0;

		virtual void deleteClientIpV4(const char* ip) const = 0;

		virtual const char* getServerIpV4() const = 0;

		virtual void deleteServerIpV4(const char* ip) const = 0;

		virtual uint16_t getClientPort() const = 0;

		virtual uint16_t getServerPort() const = 0;

		virtual const char* getRouteParameterString(const char* routeParameterName) const = 0;

		virtual int64_t getRouteParameterInteger(const char* routeParameterName) const = 0;

		virtual double getRouteParameterDouble(const char* routeParameterName) const = 0;

		virtual IDatabase* getOrCreateDatabase(const char* databaseName) = 0;

		virtual IDatabase* getDatabase(const char* databaseName) const = 0;

		virtual ~IHTTPRequest() = default;
	};
}
