#include "HTTPRequest.h"

#include "WebFrameworkHTTPNetwork.h"
#include "Utility/Memory.h"
#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	bool HTTPRequest::isWebFrameworkDynamicPages(const string& filePath)
	{
		size_t extension = filePath.find('.');

		if (extension == string::npos)
		{
			return false;
		}

		return string_view(filePath.data() + extension) == webFrameworkDynamicPagesExtension;
	}

	web::HTTPParser HTTPRequest::sendRequestToAnotherServer(const string& ip, const string& port, const string& request)
	{
		streams::IOSocketStream stream(make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPNetwork>(ip, port)));
		string response;

		stream << request;
		stream >> response;

		return web::HTTPParser(response);
	}

	HTTPRequest::HTTPRequest(SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources, sqlite::SQLiteManager& database, sockaddr& clientAddr, streams::IOSocketStream& stream) :
		session(session),
		serverReference(serverReference),
		stream(stream),
		database(database),
		clientAddr(clientAddr),
		staticResources(staticResources),
		dynamicResources(dynamicResources)
	{
		
	}

	HTTPRequest::HTTPRequest(HTTPRequest&& other) noexcept :
		HTTPRequest
		(
			other.session,
			other.serverReference,
			other.staticResources,
			other.dynamicResources,
			other.database,
			other.clientAddr,
			other.stream
		)

	{
		parser = move(other.parser);
		routeParameters = move(other.routeParameters);
	}

	HTTPRequest::HTTPRequest(const HTTPRequest& other) :
		HTTPRequest
		(
			other.session,
			other.serverReference,
			other.staticResources,
			other.dynamicResources,
			other.database,
			other.clientAddr,
			other.stream
		)
	{
		parser = utility::make_smartPointer<web::HTTPParser>(*other.parser);
		routeParameters = other.routeParameters;
	}

	const string& HTTPRequest::getRawParameters() const
	{
		return parser->getParameters();
	}

	const string& HTTPRequest::getMethod() const
	{
		return parser->getMethod();
	}

	const unordered_map<string, string>& HTTPRequest::getKeyValueParameters() const
	{
		return parser->getKeyValueParameters();
	}

	const string& HTTPRequest::getHTTPVersion() const
	{
		return parser->getHTTPVersion();
	}

	const unordered_map<string, string, web::HTTPParser::insensitiveStringHash, web::HTTPParser::insensitiveStringEqual>& HTTPRequest::getHeaders() const
	{
		return parser->getHeaders();
	}

	const string& HTTPRequest::getBody() const
	{
		return parser->getBody();
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

	unordered_map<string, string> HTTPRequest::getCookies() const
	{
		unordered_map<string, string> result;

		try
		{
			const string& cookies = parser->getHeaders().at("Cookie");
			size_t offset = 0;

			while (true)
			{
				size_t findKey = cookies.find('=', offset);
				size_t findValue = cookies.find("; ", offset);
				string::const_iterator startKey = cookies.begin() + offset;
				string::const_iterator endKey = cookies.begin() + findKey;
				string::const_iterator startValue = endKey + 1;
				string::const_iterator endValue = findValue != string::npos ? (cookies.begin() + findValue) : (cookies.end());

				result.insert(make_pair(string(startKey, endKey), string(startValue, endValue)));

				if (findValue == string::npos)
				{
					break;
				}

				offset = findValue + 2;
			}

		}
		catch (const out_of_range&)
		{

		}

		return result;
	}

	void HTTPRequest::sendAssetFile(const string& filePath, HTTPResponse& response, const unordered_map<string_view, string>& variables, bool isBinary, const string& fileName)
	{
		HTTPRequest::isWebFrameworkDynamicPages(filePath) ?
			this->sendDynamicFile(filePath, response, variables, isBinary, fileName) :
			this->sendStaticFile(filePath, response, isBinary, fileName);
	}

	void HTTPRequest::sendStaticFile(const string& filePath, HTTPResponse& response, bool isBinary, const string& fileName)
	{
		staticResources.sendStaticFile(filePath, response, isBinary, fileName);
	}

	void HTTPRequest::sendDynamicFile(const string& filePath, HTTPResponse& response, const unordered_map<string_view, string>& variables, bool isBinary, const string& fileName)
	{
		dynamicResources.sendDynamicFile(filePath, response, variables, isBinary, fileName);
	}

	void HTTPRequest::streamFile(const string& filePath, HTTPResponse& response, const string& fileName, size_t chunkSize)
	{
		filesystem::path assetFilePath(staticResources.getPathToAssets() / filePath);
		file_manager::Cache& cache = file_manager::FileManager::getInstance().getCache();
		
		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		string httpResponse = web::HTTPBuilder().
			headers
			(
				"Date", HTTPResponse::getFullDate(),
				"Server", "WebFramework-Server",
				"Content-Type", "application/octet-stream",
				"Content-Disposition", format(R"(attachment; filename="{}")", fileName),
				"Connection", "keep-alive",
				"Content-Length", filesystem::file_size(assetFilePath)
			).build();

		response.setIsValid(false);

#pragma warning(push)
#pragma warning(disable: 26800)

		try
		{
			const string& data = cache[assetFilePath];

			stream << move(httpResponse) + data;

			return;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException&)
		{

		}

		ifstream fileStream(assetFilePath, ios_base::binary);
		string chunk(chunkSize, '\0');

		streamsize dataSize = fileStream.read(chunk.data(), chunkSize).gcount();

		if (dataSize != chunkSize)
		{
			chunk.resize(dataSize);
		}

		cache.appendCache(assetFilePath, chunk);

		stream << move(httpResponse) + chunk;

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
		return parser->getJSON();
	}

	string HTTPRequest::getClientIpV4() const
	{
		return serverReference.getClientIpV4(clientAddr);
	}

	string HTTPRequest::getServerIpV4() const
	{
		return serverReference.getServerIpV4();
	}

	uint16_t HTTPRequest::getClientPort() const
	{
		return serverReference.getClientPortV4(clientAddr);
	}

	uint16_t HTTPRequest::getServerPort() const
	{
		return serverReference.getServerPortV4();
	}

	template<>
	const string& HTTPRequest::getRouteParameter<string>(const string& routeParameterName)
	{
		return get<string>(routeParameters.at(routeParameterName));
	}

	template<>
	const int64_t& HTTPRequest::getRouteParameter<int64_t>(const string& routeParameterName)
	{
		return get<int64_t>(routeParameters.at(routeParameterName));
	}

	streams::IOSocketStream& operator >> (streams::IOSocketStream& stream, HTTPRequest& request)
	{
		string data;

		stream >> data;

		if (data.find("HTTP") == string::npos)
		{
			throw web::exceptions::WebException();
		}

		request.parser = make_unique<web::HTTPParser>(data);

		return stream;
	}

	ostream& operator << (ostream& stream, const HTTPRequest& request)
	{
		const web::HTTPParser& parser = *request.parser.get();
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
