#include "HTTPRequestImplementation.h"

#include "Log.h"
#include "MultiLocalizationManager.h"

#include "BaseTCPServer.h"
#include "FileManager.h"
#include "HTTPSNetwork.h"
#include "Managers/SessionsManager.h"
#include <Managers/DatabasesManager.h>
#include <Databases/DatabaseImplementation.h>
#include <Exceptions/APIException.h>

#include "Exceptions/FileDoesNotExistException.h"

#ifndef __LINUX__
#pragma warning(disable: 6386)
#endif

using namespace std;

namespace framework
{
	bool HTTPRequestImplementation::isWebFrameworkDynamicPages(string_view filePath)
	{
		size_t extension = filePath.find('.');

		if (extension == string::npos)
		{
			return false;
		}

		return filePath.substr(extension) == webFrameworkDynamicPagesExtension;
	}

	void HTTPRequestImplementation::logWebFrameworkModelsError(string_view typeName)
	{
		if (Log::isValid())
		{
			Log::error("Can't get or create model in HTTPRequest::getModel<T> function where T is {}", "LogWebFrameworkModels", typeName);
		}
	}

	void HTTPRequestImplementation::setParser(const web::HTTPParser& parser)
	{
		this->parser = parser;
	}

	web::HTTPParser HTTPRequestImplementation::sendRequestToAnotherServer(string_view ip, string_view port, string_view request, DWORD timeout, bool useHTTPS)
	{
		streams::IOSocketStream stream = useHTTPS ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>(ip, port, chrono::milliseconds(timeout)) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>(ip, port, chrono::milliseconds(timeout));
		string response;

		stream << request;
		stream >> response;

		return web::HTTPParser(response);
	}

	HTTPRequestImplementation::HTTPRequestImplementation(SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources, sockaddr clientAddr, streams::IOSocketStream& stream) :
		session(session),
		serverReference(serverReference),
		stream(stream),
		clientAddr(clientAddr),
		staticResources(staticResources),
		dynamicResources(dynamicResources)
	{

	}

	void HTTPRequestImplementation::updateLargeData(const char* dataPart, size_t dataPartSize, size_t bodySize)
	{
		largeData.dataPart = dataPart;
		largeData.dataPartSize = dataPartSize;
		largeData.bodySize = bodySize;
		largeData.isLastPacket = !bodySize;
	}

	const char* HTTPRequestImplementation::getRawParameters() const
	{
		return parser.getParameters().data();
	}

	const char* HTTPRequestImplementation::getMethod() const
	{
		return parser.getMethod().data();
	}

	void HTTPRequestImplementation::getQueryParameters(void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer) const
	{
		const unordered_map<string, string>& queryParameters = parser.getQueryParameters();
		size_t index = 0;

		initQueryBuffer(queryParameters.size(), buffer);

		for (const auto& [key, value] : queryParameters)
		{
			addQueryParameter(key.data(), value.data(), index++, buffer);
		}
	}

	const char* HTTPRequestImplementation::getQueryParameter(const char* key) const
	{
		return parser.getQueryParameters().at(key).data();
	}

	double HTTPRequestImplementation::getHTTPVersion() const
	{
		return parser.getHTTPVersion();
	}

	void HTTPRequestImplementation::getHeaders(void(*initHeadersBuffer)(size_t size, void* buffer), void(*addHeader)(const char* key, const char* value, size_t index, void* buffer), void* buffer) const
	{
		const web::HeadersMap& headers = parser.getHeaders();
		size_t index = 0;

		initHeadersBuffer(headers.size(), buffer);

		for (const auto& [key, value] : headers)
		{
			addHeader(key.data(), value.data(), index++, buffer);
		}
	}

	const char* HTTPRequestImplementation::getHeaderValue(const char* headerName, bool throwException) const
	{
		const web::HeadersMap& headers = parser.getHeaders();

		if (throwException)
		{
			return headers.at(headerName).data();
		}

		auto it = headers.find(headerName);

		return it != headers.end() ? it->second.data() : nullptr;
	}

	const char* HTTPRequestImplementation::getBody() const
	{
		return parser.getBody().data();
	}

	void HTTPRequestImplementation::setAttribute(const char* name, const char* value)
	{
		session.setAttribute(this->getClientIpV4(), name, value);
	}

	const char* HTTPRequestImplementation::getAttribute(const char* name)
	{
		string temp = session.getAttribute(this->getClientIpV4(), name);
		char* result = new char[temp.size() + 1];

		temp.copy(result, temp.size());

		result[temp.size()] = '\0';

		return result;
	}

	void HTTPRequestImplementation::deleteAttribute(const char* attribute)
	{
		delete[] attribute;
	}

	void HTTPRequestImplementation::deleteSession()
	{
		session.deleteSession(this->getClientIpV4());
	}

	void HTTPRequestImplementation::removeAttribute(const char* name)
	{
		session.deleteAttribute(this->getClientIpV4(), name);
	}

	void HTTPRequestImplementation::getCookies(void(initCookiesBuffer)(size_t size, void* buffer), void(*addCookie)(const char* key, const char* value, size_t index, void* buffer), void* buffer) const
	{
		const web::HeadersMap& headers = parser.getHeaders();

		if (auto it = headers.find("Cookie"); it != headers.end())
		{
			const string& cookies = it->second;
			size_t offset = 0;
			size_t index = 0;
			vector<pair<string_view, string_view>> temp;

			while (true)
			{
				size_t findKey = cookies.find('=', offset);
				size_t findValue = cookies.find("; ", offset);
				string::const_iterator startKey = cookies.begin() + offset;
				string::const_iterator endKey = cookies.begin() + findKey;
				string::const_iterator startValue = endKey + 1;
				string::const_iterator endValue = findValue != string::npos ? (cookies.begin() + findValue) : (cookies.end());

				temp.emplace_back(string_view(startKey, endKey), string_view(startValue, endValue));

				if (findValue == string::npos)
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

	void HTTPRequestImplementation::getMultiparts(void(*initMultipartsBuffer)(size_t size, void* buffer), void(*addMultipart)(const char* name, const char* fileName, const char* contentType, const char* data, size_t index, void* buffer), void* buffer) const
	{
		const vector<web::Multipart>& multiparts = parser.getMultiparts();

		for (size_t i = 0; i < multiparts.size(); i++)
		{
			const web::Multipart& multipart = multiparts[i];
			interfaces::CMultipart temp;

			temp.name = multipart.getName().data();

			if (const optional<string>& fileName = multipart.getFileName())
			{
				temp.fileName = (*fileName).data();
			}

			if (const optional<string>& contentType = multipart.getContentType())
			{
				temp.contentType = (*contentType).data();
			}

			temp.data = multipart.getData().data();

			addMultipart(temp.name, temp.fileName, temp.contentType, temp.data, i, buffer);
		}
	}

	const interfaces::CLargeData* HTTPRequestImplementation::getLargeData() const
	{
		return &largeData;
	}

	void HTTPRequestImplementation::sendAssetFile(const char* filePath, interfaces::IHTTPResponse* response, size_t variablesSize, const interfaces::CVariable* variables, bool isBinary, const char* fileName)
	{
		HTTPRequestImplementation::isWebFrameworkDynamicPages(filePath) ?
			this->sendDynamicFile(filePath, response, variablesSize, variables, isBinary, fileName) :
			this->sendStaticFile(filePath, response, isBinary, fileName);
	}

	void HTTPRequestImplementation::sendStaticFile(const char* filePath, interfaces::IHTTPResponse* response, bool isBinary, const char* fileName)
	{
		staticResources.sendStaticFile(filePath, *response, isBinary, fileName);
	}

	void HTTPRequestImplementation::sendDynamicFile(const char* filePath, interfaces::IHTTPResponse* response, size_t variablesSize, const interfaces::CVariable* variables, bool isBinary, const char* fileName)
	{
		dynamicResources.sendDynamicFile(filePath, *response, span<const interfaces::CVariable>(variables, variablesSize), isBinary, fileName);
	}

	void HTTPRequestImplementation::streamFile(const char* filePath, interfaces::IHTTPResponse* response, const char* fileName, size_t chunkSize)
	{
		filesystem::path assetFilePath(staticResources.getPathToAssets() / filePath);
		file_manager::Cache& cache = file_manager::FileManager::getInstance().getCache();

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		web::HTTPBuilder builder = web::HTTPBuilder().
			headers
			(
				"Date", HTTPResponseImplementation::getFullDate(),
				"Server", "WebFramework-Server",
				"Content-Type", "application/octet-stream",
				"Content-Disposition", format(R"(attachment; filename="{}")", fileName),
				"Connection", "keep-alive",
				"Content-Length", filesystem::file_size(assetFilePath)
			).
			responseCode(web::ResponseCodes::ok);

		response->setIsValid(false);

#pragma warning(push)
#pragma warning(disable: 26800)
		if (cache.contains(assetFilePath))
		{
			const string& data = cache[assetFilePath];

			builder.headers
			(
				"DownloadType", "from-cache"
			);

			stream << builder.build(data);

			return;
		}

		ifstream fileStream(assetFilePath, ios_base::binary);
		string chunk(chunkSize, '\0');

		streamsize dataSize = fileStream.read(chunk.data(), chunkSize).gcount();

		if (dataSize != chunkSize)
		{
			chunk.resize(dataSize);
		}

		cache.appendCache(assetFilePath, chunk);

		builder.headers
		(
			"DownloadType", "from-file"
		);

		stream << builder.build() + chunk;
#pragma warning(pop)

		while (!fileStream.eof())
		{
			dataSize = fileStream.read(chunk.data(), chunkSize).gcount();

			if (dataSize != chunkSize)
			{
				chunk.resize(dataSize);
			}

			cache.appendCache(assetFilePath, chunk);

			stream << chunk;
		}
	}

	void HTTPRequestImplementation::registerDynamicFunction(const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result))
	{
		dynamicResources.registerDynamicFunction
		(
			functionName,
			[function, deleter](const vector<string>& arguments) -> string
			{
				const char** temp = new const char* [arguments.size()];

				for (size_t i = 0; i < arguments.size(); i++)
				{
					temp[i] = arguments[i].data();
				}

				const char* tempResult = function(temp, arguments.size());
				string result(tempResult);

				if (deleter)
				{
					deleter(tempResult);
				}

				delete[] temp;

				return result;
			}
		);
	}

	void HTTPRequestImplementation::unregisterDynamicFunction(const char* functionName)
	{
		dynamicResources.unregisterDynamicFunction(functionName);
	}

	bool HTTPRequestImplementation::isDynamicFunctionRegistered(const char* functionName)
	{
		return dynamicResources.isDynamicFunctionRegistered(functionName);
	}

	void HTTPRequestImplementation::sendFileChunks(interfaces::IHTTPResponse* response, const char* fileName, void* chunkGenerator, const char* (*getChunk)(void* chunkGenerator))
	{
		web::HTTPBuilder builder = web::HTTPBuilder().chunk(getChunk(chunkGenerator)).partialChunks().responseCode(web::ResponseCodes::ok).headers
		(
			"Date", HTTPResponseImplementation::getFullDate(),
			"Server", "WebFramework-Server",
			"Connection", "keep-alive"
		);

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
			const char* data = getChunk(chunkGenerator);

			stream << web::HTTPBuilder::getChunk(data ? data : "");

			if (stream.eof() || !data)
			{
				break;
			}
		}
	}

	void HTTPRequestImplementation::throwException(const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionClassHash)
	{
		throw framework::exceptions::APIException(errorMessage, responseCode, logCategory ? logCategory : "", exceptionClassHash);
	}

	void HTTPRequestImplementation::getChunks(void(*initChunkBuffer)(size_t size, void* buffer), void(*addChunk)(const char* chunk, size_t chunkSize, size_t index, void* buffer), void* buffer) const
	{
		const vector<string>& chunks = parser.getChunks();

		initChunkBuffer(chunks.size(), buffer);

		for (size_t i = 0; i < chunks.size(); i++)
		{
			const string& chunk = chunks[i];

			addChunk(chunk.data(), chunk.size(), i, buffer);
		}
	}

	const char* HTTPRequestImplementation::getJSON() const
	{
		return parser.getJSON().getRawData().data();
	}

	const char* HTTPRequestImplementation::getRawRequest() const
	{
		return parser.getRawData().data();
	}

	const char* HTTPRequestImplementation::getClientIpV4() const
	{
		string ip = web::BaseTCPServer::getClientIpV4(clientAddr);
		char* result = new char[ip.size() + 1];

		ip.copy(result, ip.size());

		result[ip.size()] = '\0';

		return result;
	}

	void HTTPRequestImplementation::deleteClientIpV4(const char* ip) const
	{
		delete[] ip;
	}

	const char* HTTPRequestImplementation::getServerIpV4() const
	{
		string ip = serverReference.getServerIpV4();
		char* result = new char[ip.size() + 1];

		ip.copy(result, ip.size());

		result[ip.size()] = '\0';

		return result;
	}

	void HTTPRequestImplementation::deleteServerIpV4(const char* ip) const
	{
		delete[] ip;
	}

	uint16_t HTTPRequestImplementation::getClientPort() const
	{
		return web::BaseTCPServer::getClientPortV4(clientAddr);
	}

	uint16_t HTTPRequestImplementation::getServerPort() const
	{
		return serverReference.getServerPortV4();
	}

	const char* HTTPRequestImplementation::getRouteParameterString(const char* routeParameterName) const
	{
		return get<string>(routeParameters.at(routeParameterName)).data();
	}

	int64_t HTTPRequestImplementation::getRouteParameterInteger(const char* routeParameterName) const
	{
		return get<int64_t>(routeParameters.at(routeParameterName));
	}

	double HTTPRequestImplementation::getRouteParameterDouble(const char* routeParameterName) const
	{
		return get<double>(routeParameters.at(routeParameterName));
	}

	interfaces::IDatabase* HTTPRequestImplementation::getOrCreateDatabase(const char* databaseName)
	{
		return databases.emplace_back(new DatabaseImplementation(DatabasesManager::get().getOrCreateDatabase(databaseName)));
	}

	interfaces::IDatabase* HTTPRequestImplementation::getDatabase(const char* databaseName) const
	{
		return databases.emplace_back(new DatabaseImplementation(DatabasesManager::get().getDatabase(databaseName)));
	}

	HTTPRequestImplementation::~HTTPRequestImplementation()
	{
		ranges::for_each(databases, [](interfaces::IDatabase* database) { delete database; });
	}

	streams::IOSocketStream& operator >> (streams::IOSocketStream& stream, HTTPRequestImplementation& request)
	{
		string data;

		stream >> data;

		request.parser.parse(data);

		return stream;
	}

	ostream& operator << (ostream& stream, const HTTPRequestImplementation& request)
	{
		const web::HTTPParser& parser = request.parser;
		const auto& headers = parser.getHeaders();

		stream << parser.getMethod() << " " << parser.getParameters() << " " << parser.getHTTPVersion() << endl;

		for (const auto& [name, value] : headers)
		{
			stream << name << ": " << value << endl;
		}

		stream << endl << parser.getBody();

		return stream;
	}
}
