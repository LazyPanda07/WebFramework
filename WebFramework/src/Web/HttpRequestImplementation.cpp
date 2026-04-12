#include "Web/HttpRequestImplementation.h"

#include <MultiLocalizationManager.h>
#include <Exceptions/FileDoesNotExistException.h>
#include <BaseTCPServer.h>
#include <FileManager.h>
#include <HttpsNetwork.h>

#include "Managers/SessionsManager.h"
#include "Managers/DatabasesManager.h"
#include "Databases/DatabaseImplementation.h"
#include "Exceptions/APIException.h"
#include "ExecutorsConstants.h"
#include "Utility/ExecutorsUtility.h"
#include "Managers/TaskBrokersManager.h"

#ifndef __LINUX__
#pragma warning(disable: 6386)
#endif

namespace framework
{
	HttpRequestImplementation::ExceptionData::ExceptionData() :
		responseCode(0),
		valid(false)
	{

	}

	void HttpRequestImplementation::ExceptionData::setLogCategory(std::string_view logCategory)
	{
		this->logCategory = logCategory.empty() ?
			"LogAPI" :
			logCategory;
	}

	std::string_view HttpRequestImplementation::ExceptionData::getLogCategory() const
	{
		return logCategory;
	}

	bool HttpRequestImplementation::isWebFrameworkDynamicPages(std::string_view filePath)
	{
		size_t extension = filePath.find('.');

		if (extension == std::string::npos)
		{
			return false;
		}

		return filePath.substr(extension) == webFrameworkDynamicPagesExtension;
	}

	void HttpRequestImplementation::setParser(const web::HttpParser& parser)
	{
		this->parser = parser;
	}

	web::HttpParser HttpRequestImplementation::sendRequestToAnotherServer(std::string_view ip, std::string_view port, std::string_view request, DWORD timeout, bool useHTTPS)
	{
		streams::IOSocketStream stream = useHTTPS ?
			streams::IOSocketStream::createStream<web::HttpsNetwork>(ip, port, std::chrono::milliseconds(timeout)) :
			streams::IOSocketStream::createStream<web::HttpNetwork>(ip, port, std::chrono::milliseconds(timeout));
		std::string response;

		stream << request;
		stream >> response;

		return web::HttpParser(response);
	}

	void HttpRequestImplementation::registerDynamicFunctionClassStatic(const char* functionName, const char* apiType, void* functionClass, interfaces::IDynamicFile& dynamicResources)
	{
		if (apiType == json_settings::cxxExecutorKey || apiType == json_settings::ccExecutorKey)
		{
			dynamicResources.registerDynamicFunction(functionName, apiType, functionClass);
		}

#ifdef __WITH_PYTHON_EXECUTORS__
		if (apiType == json_settings::pythonExecutorKey)
		{
			dynamicResources.registerDynamicFunction(functionName, apiType, functionClass);
		}
#endif
#ifdef __WITH_DOTNET_EXECUTORS__
		if (apiType == json_settings::csharpExecutorKey)
		{
			dynamicResources.registerDynamicFunction(functionName, apiType, static_cast<char*>(functionClass));
		}
#endif
	}

	HttpRequestImplementation::HttpRequestImplementation(SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources, sockaddr clientAddr, streams::IOSocketStream& stream) :
		session(session),
		serverReference(serverReference),
		stream(stream),
		clientAddr(clientAddr),
		staticResources(staticResources),
		dynamicResources(dynamicResources)
	{

	}

	void HttpRequestImplementation::updateLargeData(const char* dataPart, size_t dataPartSize, bool isLast)
	{
		largeData.dataPart = dataPart;
		largeData.dataPartSize = dataPartSize;
		largeData.isLastPacket = isLast;
	}

	const char* HttpRequestImplementation::getRawParameters() const
	{
		return parser.getParameters().data();
	}

	const char* HttpRequestImplementation::getMethod() const
	{
		return parser.getMethod().data();
	}

	void HttpRequestImplementation::getQueryParameters(void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer) const
	{
		const std::unordered_map<std::string, std::string>& queryParameters = parser.getQueryParameters();
		size_t index = 0;

		initQueryBuffer(queryParameters.size(), buffer);

		for (const auto& [key, value] : queryParameters)
		{
			addQueryParameter(key.data(), value.data(), index++, buffer);
		}
	}

	const char* HttpRequestImplementation::getQueryParameter(const char* key) const
	{
		return parser.getQueryParameters().at(key).data();
	}

	double HttpRequestImplementation::getHTTPVersion() const
	{
		return parser.getHTTPVersion();
	}

	void HttpRequestImplementation::getHeaders(void(*initHeadersBuffer)(size_t size, void* buffer), void(*addHeader)(const char* key, const char* value, size_t index, void* buffer), void* buffer) const
	{
		const web::HeadersMap& headers = parser.getHeaders();
		size_t index = 0;

		initHeadersBuffer(headers.size(), buffer);

		for (const auto& [key, value] : headers)
		{
			addHeader(key.data(), value.data(), index++, buffer);
		}
	}

	const char* HttpRequestImplementation::getHeaderValue(const char* headerName, bool throwException) const
	{
		const web::HeadersMap& headers = parser.getHeaders();

		if (throwException)
		{
			return headers.at(headerName).data();
		}

		auto it = headers.find(headerName);

		return it != headers.end() ? it->second.data() : nullptr;
	}

	const char* HttpRequestImplementation::getBody(size_t* bodySize) const
	{
		*bodySize = parser.getBody().size();

		return parser.getBody().data();
	}

	void HttpRequestImplementation::setAttribute(const char* name, const char* value)
	{
		session.setAttribute(this->getClientIpV4(), name, value);
	}

	const char* HttpRequestImplementation::getAttribute(const char* name)
	{
		std::string temp = session.getAttribute(this->getClientIpV4(), name);
		char* result = new char[temp.size() + 1];

		temp.copy(result, temp.size());

		result[temp.size()] = '\0';

		return result;
	}

	void HttpRequestImplementation::deleteAttribute(const char* attribute)
	{
		delete[] attribute;
	}

	void HttpRequestImplementation::deleteSession()
	{
		session.deleteSession(this->getClientIpV4());
	}

	void HttpRequestImplementation::removeAttribute(const char* name)
	{
		session.deleteAttribute(this->getClientIpV4(), name);
	}

	void HttpRequestImplementation::getCookies(void(initCookiesBuffer)(size_t size, void* buffer), void(*addCookie)(const char* key, const char* value, size_t index, void* buffer), void* buffer) const
	{
		const web::HeadersMap& headers = parser.getHeaders();

		if (auto it = headers.find("Cookie"); it != headers.end())
		{
			const std::string& cookies = it->second;
			size_t offset = 0;
			size_t index = 0;
			std::vector<std::pair<std::string_view, std::string_view>> temp;

			while (true)
			{
				size_t findKey = cookies.find('=', offset);
				size_t findValue = cookies.find("; ", offset);
				std::string::const_iterator startKey = cookies.begin() + offset;
				std::string::const_iterator endKey = cookies.begin() + findKey;
				std::string::const_iterator startValue = endKey + 1;
				std::string::const_iterator endValue = findValue != std::string::npos ? (cookies.begin() + findValue) : (cookies.end());

				temp.emplace_back(std::string_view(startKey, endKey), std::string_view(startValue, endValue));

				if (findValue == std::string::npos)
				{
					break;
				}

				offset = findValue + 2;
			}

			initCookiesBuffer(temp.size(), buffer);

			for (const auto& [key, value] : temp)
			{
				addCookie(key.data(), value.data(), index++, buffer);
			}
		}
	}

	void HttpRequestImplementation::getMultiparts(void(*initMultipartsBuffer)(size_t size, void* buffer), void(*addMultipart)(const char* name, const char* fileName, const char* contentType, const char* data, size_t dataSize, size_t index, void* buffer), void* buffer) const
	{
		const std::vector<web::Multipart>& multiparts = parser.getMultiparts();

		initMultipartsBuffer(multiparts.size(), buffer);

		for (size_t i = 0; i < multiparts.size(); i++)
		{
			const web::Multipart& multipart = multiparts[i];
			interfaces::CMultipart temp;

			temp.name = multipart.getName().data();

			if (const std::optional<std::string>& fileName = multipart.getFileName())
			{
				temp.fileName = (*fileName).data();
			}

			if (const std::optional<std::string>& contentType = multipart.getContentType())
			{
				temp.contentType = (*contentType).data();
			}

			size_t dataSize = multipart.getData().size();
			temp.data = multipart.getData().data();

			addMultipart(temp.name, temp.fileName, temp.contentType, temp.data, dataSize, i, buffer);
		}
	}

	const interfaces::CLargeData* HttpRequestImplementation::getLargeData() const
	{
		return &largeData;
	}

	void HttpRequestImplementation::sendAssetFile(const char* filePath, interfaces::IHttpResponse* response, const void* arguments, const char* fileName)
	{
		HttpRequestImplementation::isWebFrameworkDynamicPages(filePath) ?
			this->sendDynamicFile(filePath, response, arguments, fileName) :
			this->sendStaticFile(filePath, response, fileName);
	}

	void HttpRequestImplementation::sendStaticFile(const char* filePath, interfaces::IHttpResponse* response, const char* fileName)
	{
		staticResources.sendStaticFile(filePath, *response, fileName);
	}

	void HttpRequestImplementation::sendDynamicFile(const char* filePath, interfaces::IHttpResponse* response, const void* arguments, const char* fileName)
	{
		dynamicResources.sendDynamicFile(filePath, *response, arguments, fileName);
	}

	void HttpRequestImplementation::streamFile(const char* filePath, interfaces::IHttpResponse* response, const char* fileName, size_t chunkSize)
	{
		std::unique_ptr<std::istream> fileStream = staticResources.getFileStream(filePath);
		std::streamsize fileSize = fileStream->tellg();

		fileStream->seekg(0, std::ios::end);

		fileSize = fileStream->tellg() - fileSize;

		fileStream->seekg(0, std::ios::beg);

		web::HttpBuilder builder = web::HttpBuilder().
			headers
			(
				"Date", HttpResponseImplementation::getFullDate(),
				"Server", "WebFramework-Server",
				"Content-Type", "application/octet-stream",
				"Content-Disposition", std::format(R"(attachment; filename="{}")", fileName),
				"Connection", "keep-alive",
				"Content-Length", fileSize
			).
			responseCode(web::ResponseCodes::ok);

		response->setIsValid(false);

		std::string chunk(chunkSize, '\0');
		std::streamsize dataSize = fileStream->read(chunk.data(), chunkSize).gcount();

		if (dataSize != chunkSize)
		{
			chunk.resize(dataSize);
		}

		stream << builder.build() + chunk;

		while (!fileStream->eof())
		{
			dataSize = fileStream->read(chunk.data(), chunkSize).gcount();

			if (dataSize != chunkSize)
			{
				chunk.resize(dataSize);
			}

			stream << chunk;
		}	
	}

	void HttpRequestImplementation::registerDynamicFunctionClass(const char* functionName, const char* apiType, void* functionClass)
	{
		HttpRequestImplementation::registerDynamicFunctionClassStatic(functionName, apiType, functionClass, dynamicResources);
	}

	void HttpRequestImplementation::unregisterDynamicFunction(const char* functionName)
	{
		dynamicResources.unregisterDynamicFunction(functionName);
	}

	bool HttpRequestImplementation::isDynamicFunctionRegistered(const char* functionName)
	{
		return dynamicResources.isDynamicFunctionRegistered(functionName);
	}

	void HttpRequestImplementation::sendFileChunks(interfaces::IHttpResponse* response, const char* fileName, void* chunkGenerator, const char* (*getChunk)(void* chunkGenerator, size_t* size))
	{
		web::HttpBuilder builder;

		{
			size_t chunkSize = 0;
			const char* chunk = getChunk(chunkGenerator, &chunkSize);

			builder = web::HttpBuilder().chunk(std::string_view(chunk, chunkSize)).partialChunks().responseCode(web::ResponseCodes::ok).headers
			(
				"Date", HttpResponseImplementation::getFullDate(),
				"Server", "WebFramework-Server",
				"Connection", "keep-alive"
			);
		}

		if (fileName)
		{
			builder.headers
			(
				"Content-Disposition", std::format(R"(attachment; filename="{}")", fileName)
			);
		}

		stream << builder.build();

		response->setIsValid(false);

		while (true)
		{
			size_t chunkSize = 0;
			const char* chunk = getChunk(chunkGenerator, &chunkSize);

			if (chunkSize)
			{
				stream << web::HttpBuilder::getChunk(std::string_view(chunk, chunkSize));
			}
			else
			{
				stream << web::HttpBuilder::getChunk("");

				break;
			}

			if (stream.eof() || !chunkSize)
			{
				break;
			}
		}
	}

	void HttpRequestImplementation::throwException(const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionClassHash)
	{
		throw framework::exceptions::APIException(errorMessage, responseCode, logCategory ? logCategory : "", exceptionClassHash);
	}

	void HttpRequestImplementation::getChunks(void(*initChunkBuffer)(size_t size, void* buffer), void(*addChunk)(const char* chunk, size_t chunkSize, size_t index, void* buffer), void* buffer) const
	{
		const std::vector<std::string>& chunks = parser.getChunks();

		initChunkBuffer(chunks.size(), buffer);

		for (size_t i = 0; i < chunks.size(); i++)
		{
			const std::string& chunk = chunks[i];

			addChunk(chunk.data(), chunk.size(), i, buffer);
		}
	}

	void HttpRequestImplementation::getFile(const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer) const
	{
		std::string data = staticResources.getFile(filePath);

		fillBuffer(data.data(), data.size(), buffer);
	}

	void HttpRequestImplementation::processStaticFile(const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer)
	{
		const std::unique_ptr<interfaces::IStaticFileRenderer>& renderer = staticResources.getStaticRenderers().at(fileExtension);
		std::string result = renderer->render(std::string_view(fileData, size));

		fillBuffer(result.data(), result.size(), buffer);
	}

	void HttpRequestImplementation::processDynamicFile(const char* fileData, size_t size, const void* arguments, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer)
	{
		std::string result(fileData, size);

		dynamicResources.processDynamicFile(result, arguments);

		fillBuffer(result.data(), result.size(), buffer);
	}

	void HttpRequestImplementation::enqueueTask(const char* messageBrokerName, void* jsonObjectData)
	{
		json::JsonObject& data = *static_cast<json::JsonObject*>(jsonObjectData);
		framework::task_broker::TaskBrokersManager& manager = framework::task_broker::TaskBrokersManager::get();

		manager.getTaskBroker(messageBrokerName).enqueueTask(std::move(data));
	}

	void HttpRequestImplementation::setExceptionData(const char* errorMessage, int responseCode, const char* logCategory)
	{
		exceptionData.errorMessage = errorMessage;
		exceptionData.responseCode = responseCode;
		exceptionData.valid = true;

		exceptionData.setLogCategory(logCategory ? logCategory : "");
	}

	bool HttpRequestImplementation::isExceptionDataValid() const
	{
		return exceptionData.valid;
	}

	bool HttpRequestImplementation::getExceptionData(interfaces::CExceptionData* data)
	{
		if (exceptionData.valid)
		{
			data->errorMessage = exceptionData.errorMessage.data();
			data->responseCode = exceptionData.responseCode;
			data->logCategory = exceptionData.getLogCategory().data();

			exceptionData.valid = false;

			return true;
		}

		return false;
	}

	const char* HttpRequestImplementation::getJson() const
	{
		return parser.getJson().getRawData().data();
	}

	const char* HttpRequestImplementation::getRawRequest() const
	{
		return parser.getRawData().data();
	}

	const char* HttpRequestImplementation::getClientIpV4() const
	{
		std::string ip = web::BaseTCPServer::getClientIpV4(clientAddr);
		char* result = new char[ip.size() + 1];

		ip.copy(result, ip.size());

		result[ip.size()] = '\0';

		return result;
	}

	void HttpRequestImplementation::deleteClientIpV4(const char* ip) const
	{
		delete[] ip;
	}

	const char* HttpRequestImplementation::getServerIpV4() const
	{
		std::string ip = serverReference.getServerIpV4();
		char* result = new char[ip.size() + 1];

		ip.copy(result, ip.size());

		result[ip.size()] = '\0';

		return result;
	}

	void HttpRequestImplementation::deleteServerIpV4(const char* ip) const
	{
		delete[] ip;
	}

	uint16_t HttpRequestImplementation::getClientPort() const
	{
		return web::BaseTCPServer::getClientPortV4(clientAddr);
	}

	uint16_t HttpRequestImplementation::getServerPort() const
	{
		return serverReference.getServerPortV4();
	}

	const char* HttpRequestImplementation::getRouteStringParameter(const char* routeParameterName) const
	{
		return std::get<std::string>(routeParameters.at(routeParameterName)).data();
	}

	int64_t HttpRequestImplementation::getRouteIntegerParameter(const char* routeParameterName) const
	{
		return std::get<int64_t>(routeParameters.at(routeParameterName));
	}

	double HttpRequestImplementation::getRouteDoubleParameter(const char* routeParameterName) const
	{
		return std::get<double>(routeParameters.at(routeParameterName));
	}

	interfaces::IDatabase* HttpRequestImplementation::getOrCreateDatabase(const char* databaseName, const char* databaseImplementationName)
	{
		return databases.emplace_back(new DatabaseImplementation(DatabasesManager::get().getOrCreateDatabase(databaseName, databaseImplementationName)));
	}

	interfaces::IDatabase* HttpRequestImplementation::getDatabase(const char* databaseName, const char* databaseImplementationName) const
	{
		return databases.emplace_back(new DatabaseImplementation(DatabasesManager::get().getDatabase(databaseName, databaseImplementationName)));
	}

	HttpRequestImplementation::~HttpRequestImplementation()
	{
		std::ranges::for_each(databases, [](interfaces::IDatabase* database) { delete database; });
	}

	streams::IOSocketStream& operator >> (streams::IOSocketStream& stream, HttpRequestImplementation& request)
	{
		std::string data;

		stream >> data;

		request.parser = web::HttpParser(data);

		return stream;
	}

	std::ostream& operator << (std::ostream& stream, const HttpRequestImplementation& request)
	{
		const web::HttpParser& parser = request.parser;
		const auto& headers = parser.getHeaders();

		stream << parser.getMethod() << " " << parser.getParameters() << " " << parser.getHTTPVersion() << std::endl;

		for (const auto& [name, value] : headers)
		{
			stream << name << ": " << value << std::endl;
		}

		stream << std::endl << parser.getBody();

		return stream;
	}
}
