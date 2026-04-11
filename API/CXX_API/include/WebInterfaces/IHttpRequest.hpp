#pragma once

#include "IHttpResponse.hpp"

#include "DatabaseInterfaces/IDatabase.hpp"

namespace framework::interfaces
{
	struct CLargeData
	{
		const char* dataPart;
		size_t dataPartSize;
		bool isLastPacket;

		CLargeData() :
			dataPart(nullptr),
			dataPartSize(0),
			isLastPacket(true)
		{

		}
	};

	struct CMultipart
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

	struct CExceptionData
	{
	public:
		const char* errorMessage;
		int responseCode;
		const char* logCategory;

	public:
		CExceptionData() :
			errorMessage(nullptr),
			responseCode(0),
			logCategory(nullptr)
		{

		}

		~CExceptionData() = default;
	};

	class IHttpRequest
	{
	public:
		static inline constexpr size_t defaultChunkSize = 14 * 1024 * 1024;

	public:
		IHttpRequest() = default;

		virtual void updateLargeData(const char* dataPart, size_t dataPartSize, bool isLast) = 0;

		virtual const char* getRawParameters() const = 0;

		virtual const char* getMethod() const = 0;

		virtual void getQueryParameters(void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer) const = 0;

		virtual const char* getQueryParameter(const char* key) const = 0;

		virtual double getHTTPVersion() const = 0;

		virtual void getHeaders(void(*initHeadersBuffer)(size_t size, void* buffer), void(*addHeader)(const char* key, const char* value, size_t index, void* buffer), void* buffer) const = 0;

		virtual const char* getHeaderValue(const char* headerName, bool throwException = true) const = 0;

		virtual const char* getBody(size_t* bodySize) const = 0;

		virtual void setAttribute(const char* name, const char* value) = 0;

		virtual const char* getAttribute(const char* name) = 0;

		virtual void deleteAttribute(const char* attribute) = 0;

		virtual void deleteSession() = 0;

		virtual void removeAttribute(const char* name) = 0;

		virtual void getCookies(void(*initCookiesBuffer)(size_t size, void* buffer), void(*addCookie)(const char* key, const char* value, size_t index, void* buffer), void* buffer) const = 0;

		virtual void getMultiparts(void(*initMultipartsBuffer)(size_t size, void* buffer), void(*addMultipart)(const char* name, const char* fileName, const char* contentType, const char* data, size_t dataSize, size_t index, void* buffer), void* buffer) const = 0;

		virtual const CLargeData* getLargeData() const = 0;

		virtual void sendAssetFile(const char* filePath, IHttpResponse* response, const void* arguments = nullptr, const char* fileName = "") = 0;

		virtual void sendStaticFile(const char* filePath, IHttpResponse* response, const char* fileName = "") = 0;

		virtual void sendDynamicFile(const char* filePath, IHttpResponse* response, const void* arguments, const char* fileName = "") = 0;

		virtual void streamFile(const char* filePath, IHttpResponse* response, const char* fileName, size_t chunkSize = IHttpRequest::defaultChunkSize) = 0;

		virtual void registerDynamicFunctionClass(const char* functionName, const char* apiType, void* functionClass) = 0;

		virtual void unregisterDynamicFunction(const char* functionName) = 0;

		virtual bool isDynamicFunctionRegistered(const char* functionName) = 0;

		virtual void sendFileChunks(IHttpResponse* response, const char* fileName, void* chunkGenerator, const char* (*addChunk)(void* chunkGenerator, size_t* chunkSize)) = 0;

		virtual void throwException(const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionClassHash) = 0;

		virtual void getChunks(void(*initChunkBuffer)(size_t size, void* buffer), void(*addChunk)(const char* chunk, size_t chunkSize, size_t index, void* buffer), void* buffer) const = 0;

		virtual void getFile(const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer) const = 0;

		virtual void processStaticFile(const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer) = 0;

		virtual void processDynamicFile(const char* fileData, size_t size, const void* arguments, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer) = 0;

		virtual void enqueueTask(const char* messageBrokerName, void* jsonObjectData) = 0;

		virtual void setExceptionData(const char* errorMessage, int responseCode, const char* logCategory) = 0;

		virtual bool isExceptionDataValid() const = 0;

		virtual bool getExceptionData(CExceptionData* data) = 0;

		virtual const char* getJson() const = 0;

		virtual const char* getRawRequest() const = 0;

		virtual const char* getClientIpV4() const = 0;

		virtual void deleteClientIpV4(const char* ip) const = 0;

		virtual const char* getServerIpV4() const = 0;

		virtual void deleteServerIpV4(const char* ip) const = 0;

		virtual uint16_t getClientPort() const = 0;

		virtual uint16_t getServerPort() const = 0;

		virtual const char* getRouteStringParameter(const char* routeParameterName) const = 0;

		virtual int64_t getRouteIntegerParameter(const char* routeParameterName) const = 0;

		virtual double getRouteDoubleParameter(const char* routeParameterName) const = 0;

		virtual IDatabase* getOrCreateDatabase(const char* databaseName, const char* databaseImplementationName) = 0;

		virtual IDatabase* getDatabase(const char* databaseName, const char* databaseImplementationName) const = 0;

		virtual ~IHttpRequest() = default;
	};
}
