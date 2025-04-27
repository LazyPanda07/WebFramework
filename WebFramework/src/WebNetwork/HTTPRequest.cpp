#include "HTTPRequest.h"

#include "Log.h"
#include "MultiLocalizationManager.h"

#include "BaseTCPServer.h"
#include "FileManager.h"
#include "HTTPSNetwork.h"
#include "Managers/SessionsManager.h"
#include "HTTPResponse.h"

#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	bool HTTPRequest::isWebFrameworkDynamicPages(string_view filePath)
	{
		size_t extension = filePath.find('.');

		if (extension == string::npos)
		{
			return false;
		}

		return filePath.substr(extension) == webFrameworkDynamicPagesExtension;
	}

	void HTTPRequest::logWebFrameworkModelsError(string_view typeName)
	{
		if (Log::isValid())
		{
			Log::error("Can't get or create model in HTTPRequest::getModel<T> function where T is {}", "LogWebFrameworkModels", typeName);
		}
	}

	web::HTTPParser HTTPRequest::sendRequestToAnotherServer(string_view ip, string_view port, string_view request, DWORD timeout, bool useHTTPS)
	{
		streams::IOSocketStream stream = useHTTPS ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>(ip, port, timeout) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>(ip, port, timeout);
		string response;

		stream << request;
		stream >> response;

		return web::HTTPParser(response);
	}

	HTTPRequest::HTTPRequest(SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources, sqlite::SQLiteManager& database, sockaddr clientAddr, streams::IOSocketStream& stream) :
		session(session),
		serverReference(serverReference),
		stream(stream),
		database(database),
		clientAddr(clientAddr),
		staticResources(staticResources),
		dynamicResources(dynamicResources)
	{

	}

	void HTTPRequest::updateLargeData(const char* dataPart, size_t dataPartSize, size_t bodySize)
	{
		largeData.dataPart = dataPart;
		largeData.dataPartSize = dataPartSize;
		largeData.bodySize = bodySize;
		largeData.isLastPacket = !bodySize;
	}

	const char* HTTPRequest::getRawParameters() const
	{
		return parser.getParameters().data();
	}

	const char* HTTPRequest::getMethod() const
	{
		return parser.getMethod().data();
	}

	const char* HTTPRequest::getKeyValueParameter(const char* key) const
	{
		return parser.getKeyValueParameters().at(key).data();
	}

	double HTTPRequest::getHTTPVersion() const
	{
		return parser.getHTTPVersion();
	}

	const char* HTTPRequest::getHeaderValue(const char* headerName) const
	{
		return parser.getHeaders().at(headerName).data();
	}

	const char* HTTPRequest::getBody() const
	{
		return parser.getBody().data();
	}

	void HTTPRequest::setAttribute(const char* name, const char* value)
	{
		session.setAttribute(this->getClientIpV4(), name, value);
	}

	const char* HTTPRequest::getAttribute(const char* name)
	{
		string temp = session.getAttribute(this->getClientIpV4(), name);
		char* result = new char[temp.size() + 1];

		temp.copy(result, temp.size());

		result[temp.size()] = '\0';

		return result;
	}

	void HTTPRequest::deleteAttribute(const char* attribute)
	{
		delete[] attribute;
	}

	void HTTPRequest::deleteSession()
	{
		session.deleteSession(this->getClientIpV4());
	}

	void HTTPRequest::removeAttribute(const char* name)
	{
		session.deleteAttribute(this->getClientIpV4(), name);
	}

	void HTTPRequest::getCookies(void(*addCookie)(const char* key, const char* value)) const
	{
		const web::HeadersMap& headers = parser.getHeaders();

		if (auto it = headers.find("Cookie"); it != headers.end())
		{
			const string& cookies = it->second;
			size_t offset = 0;

			while (true)
			{
				size_t findKey = cookies.find('=', offset);
				size_t findValue = cookies.find("; ", offset);
				string::const_iterator startKey = cookies.begin() + offset;
				string::const_iterator endKey = cookies.begin() + findKey;
				string::const_iterator startValue = endKey + 1;
				string::const_iterator endValue = findValue != string::npos ? (cookies.begin() + findValue) : (cookies.end());

				addCookie(string_view(startKey, endKey).data(), string_view(startValue, endValue).data());

				if (findValue == string::npos)
				{
					break;
				}

				offset = findValue + 2;
			}
		}
	}

	void HTTPRequest::getMultiparts(void(*addMultipart)(const interfaces::CMultipart* part)) const
	{
		const vector<web::Multipart>& multiparts = parser.getMultiparts();

		for (const web::Multipart& multipart : multiparts)
		{
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

			addMultipart(&temp);
		}
	}

	const interfaces::CLargeData* HTTPRequest::getLargeData() const
	{
		return &largeData;
	}

	void HTTPRequest::sendAssetFile(const char* filePath, interfaces::IHTTPResponse* response, size_t variablesSize, const interfaces::CVariable* variables, bool isBinary, const char* fileName)
	{
		HTTPRequest::isWebFrameworkDynamicPages(filePath) ?
			this->sendDynamicFile(filePath, response, variablesSize, variables, isBinary, fileName) :
			this->sendStaticFile(filePath, response, isBinary, fileName);
	}

	void HTTPRequest::sendStaticFile(const char* filePath, interfaces::IHTTPResponse* response, bool isBinary, const char* fileName)
	{
		staticResources.sendStaticFile(filePath, *response, isBinary, fileName);
	}

	void HTTPRequest::sendDynamicFile(const char* filePath, interfaces::IHTTPResponse* response, size_t variablesSize, const interfaces::CVariable* variables, bool isBinary, const char* fileName)
	{
		dynamicResources.sendDynamicFile(filePath, *response, variablesSize, variables, isBinary, fileName);
	}

	void HTTPRequest::streamFile(const char* filePath, interfaces::IHTTPResponse* response, const char* fileName, size_t chunkSize)
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
				"Date", HTTPResponse::getFullDate(),
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

	/*void HTTPRequest::registerDynamicFunction(string_view functionName, function<string(const vector<string>&)>&& function)
	{
		dynamicResources.registerDynamicFunction(functionName, function);
	}*/

	void HTTPRequest::unregisterDynamicFunction(const char* functionName)
	{
		dynamicResources.unregisterDynamicFunction(functionName);
	}

	bool HTTPRequest::isDynamicFunctionRegistered(const char* functionName)
	{
		return dynamicResources.isDynamicFunctionRegistered(functionName);
	}

	void HTTPRequest::getChunks(void(*getChunk)(const char* chunk, size_t chunkSize)) const
	{
		ranges::for_each(parser.getChunks(), [getChunk](const string& chunk) { getChunk(chunk.data(), chunk.size()); });
	}

	const char* HTTPRequest::getJSON() const
	{
		return parser.getJSON().getRawData().data();
	}

	const char* HTTPRequest::getRawRequest() const
	{
		return parser.getRawData().data();
	}

	const char* HTTPRequest::getClientIpV4() const
	{
		string ip = web::BaseTCPServer::getClientIpV4(clientAddr);
		char* result = new char[ip.size() + 1];

		ip.copy(result, ip.size());

		result[ip.size()] = '\0';

		return result;
	}

	void HTTPRequest::deleteClientIpV4(const char* ip) const
	{
		delete[] ip;
	}

	const char* HTTPRequest::getServerIpV4() const
	{
		string ip = serverReference.getServerIpV4();
		char* result = new char[ip.size() + 1];

		ip.copy(result, ip.size());

		result[ip.size()] = '\0';

		return result;
	}

	void HTTPRequest::deleteServerIpV4(const char* ip) const
	{
		delete[] ip;
	}

	uint16_t HTTPRequest::getClientPort() const
	{
		return web::BaseTCPServer::getClientPortV4(clientAddr);
	}

	uint16_t HTTPRequest::getServerPort() const
	{
		return serverReference.getServerPortV4();
	}

	template<>
	WEB_FRAMEWORK_CORE_API const string& HTTPRequest::getRouteParameter<string>(const string& routeParameterName)
	{
		return get<string>(routeParameters.at(routeParameterName));
	}

	template<>
	WEB_FRAMEWORK_CORE_API const int64_t& HTTPRequest::getRouteParameter<int64_t>(const string& routeParameterName)
	{
		return get<int64_t>(routeParameters.at(routeParameterName));
	}

	template<>
	WEB_FRAMEWORK_CORE_API const double& HTTPRequest::getRouteParameter<double>(const string& routeParameterName)
	{
		return get<double>(routeParameters.at(routeParameterName));
	}

	streams::IOSocketStream& operator >> (streams::IOSocketStream& stream, HTTPRequest& request)
	{
		string data;

		stream >> data;

		request.parser.parse(data);

		return stream;
	}

	ostream& operator << (ostream& stream, const HTTPRequest& request)
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
