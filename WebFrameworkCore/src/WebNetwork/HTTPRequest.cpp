#include "HTTPRequest.h"

#include "Log.h"
#include "MultiLocalizationManager.h"

#include "BaseTCPServer.h"
#include "FileManager.h"
#include "HTTPSNetwork.h"
#include "Managers/SessionsManager.h"

#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	LargeData::LargeData() :
		size(0),
		isLastPacket(true)
	{

	}

	bool HTTPRequest::isWebFrameworkDynamicPages(const string& filePath)
	{
		size_t extension = filePath.find('.');

		if (extension == string::npos)
		{
			return false;
		}

		return string_view(filePath.data() + extension) == webFrameworkDynamicPagesExtension;
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

	void HTTPRequest::updateLargeData(string_view dataPart, size_t size)
	{
		largeData.dataPart = dataPart;
		largeData.size = size;
		largeData.isLastPacket = !size;
	}

	const string& HTTPRequest::getRawParameters() const
	{
		return parser.getParameters();
	}

	const string& HTTPRequest::getMethod() const
	{
		return parser.getMethod();
	}

	const unordered_map<string, string>& HTTPRequest::getKeyValueParameters() const
	{
		return parser.getKeyValueParameters();
	}

	string HTTPRequest::getHTTPVersion() const
	{
		return "HTTP/" + to_string(parser.getHTTPVersion());
	}

	const web::HeadersMap& HTTPRequest::getHeaders() const
	{
		return parser.getHeaders();
	}

	const string& HTTPRequest::getBody() const
	{
		return parser.getBody();
	}

	void HTTPRequest::setAttribute(const string& name, const string& value)
	{
		session.setAttribute(this->getClientIpV4(), name, value);
	}

	string HTTPRequest::getAttribute(const string& name)
	{
		return session.getAttribute(this->getClientIpV4(), name);
	}

	void HTTPRequest::deleteSession()
	{
		session.deleteSession(this->getClientIpV4());
	}

	void HTTPRequest::deleteAttribute(const string& name)
	{
		session.deleteAttribute(this->getClientIpV4(), name);
	}

	web::HeadersMap HTTPRequest::getCookies() const
	{
		web::HeadersMap result;
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

				result.try_emplace(string(startKey, endKey), string(startValue, endValue));

				if (findValue == string::npos)
				{
					break;
				}

				offset = findValue + 2;
			}
		}

		return result;
	}

	const vector<web::Multipart>& HTTPRequest::getMultiparts() const
	{
		return parser.getMultiparts();
	}

	const LargeData& HTTPRequest::getLargeData() const
	{
		return largeData;
	}

	void HTTPRequest::sendAssetFile(const string& filePath, HTTPResponse& response, const unordered_map<string, string>& variables, bool isBinary, const string& fileName)
	{
		HTTPRequest::isWebFrameworkDynamicPages(filePath) ?
			this->sendDynamicFile(filePath, response, variables, isBinary, fileName) :
			this->sendStaticFile(filePath, response, isBinary, fileName);
	}

	void HTTPRequest::sendStaticFile(const string& filePath, HTTPResponse& response, bool isBinary, const string& fileName)
	{
		staticResources.sendStaticFile(filePath, response, isBinary, fileName);
	}

	void HTTPRequest::sendDynamicFile(const string& filePath, HTTPResponse& response, const unordered_map<string, string>& variables, bool isBinary, const string& fileName)
	{
		dynamicResources.sendDynamicFile(filePath, response, variables, isBinary, fileName);
	}

	void HTTPRequest::streamFile(string_view filePath, HTTPResponse& response, string_view fileName, size_t chunkSize)
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

		response.setIsValid(false);

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

	void HTTPRequest::registerDynamicFunction(const string& functionName, function<string(const vector<string>&)>&& function)
	{
		dynamicResources.registerDynamicFunction(functionName, move(function));
	}

	void HTTPRequest::unregisterDynamicFunction(const string& functionName)
	{
		dynamicResources.unregisterDynamicFunction(functionName);
	}

	bool HTTPRequest::isDynamicFunctionRegistered(const string& functionName)
	{
		return dynamicResources.isDynamicFunctionRegistered(functionName);
	}

	const json::JSONParser& HTTPRequest::getJSON() const
	{
		return parser.getJSON();
	}

	const vector<string>& HTTPRequest::getChunks() const
	{
		return parser.getChunks();
	}

	const web::HTTPParser& HTTPRequest::getParser() const
	{
		return parser;
	}

	string HTTPRequest::getClientIpV4() const
	{
		return web::BaseTCPServer::getClientIpV4(clientAddr);
	}

	string HTTPRequest::getServerIpV4() const
	{
		return serverReference.getServerIpV4();
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
