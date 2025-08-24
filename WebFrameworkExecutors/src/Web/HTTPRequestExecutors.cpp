#include "HTTPRequestExecutors.h"

using namespace std;

namespace framework
{
	LargeData::LargeData(string_view dataPart, size_t size, bool isLastPacket) :
		dataPart(dataPart),
		size(size),
		isLastPacket(isLastPacket)
	{

	}

	void HTTPRequestExecutors::initHeaders()
	{
		auto initHeadersBuffer = [](size_t size, void* buffer)
			{
				reinterpret_cast<web::HeadersMap*>(buffer)->reserve(size);
			};
		auto addHeader = [](const char* key, const char* value, size_t index, void* buffer)
			{
				reinterpret_cast<web::HeadersMap*>(buffer)->try_emplace(key, value);
			};

		implementation->getHeaders(initHeadersBuffer, addHeader, &headers);
	}

	void HTTPRequestExecutors::initQueryParameters()
	{
		auto initQueryBuffer = [](size_t size, void* buffer)
			{
				reinterpret_cast<std::unordered_map<std::string, std::string>*>(buffer)->reserve(size);
			};
		auto addQueryParameter = [](const char* key, const char* value, size_t index, void* buffer)
			{
				reinterpret_cast<std::unordered_map<std::string, std::string>*>(buffer)->try_emplace(key, value);
			};

		implementation->getQueryParameters(initQueryBuffer, addQueryParameter, &queryParameters);
	}

	void HTTPRequestExecutors::initMultiparts()
	{
		auto initMultipartsBuffer = [](size_t size, void* buffer)
			{
				static_cast<std::vector<web::Multipart>*>(buffer)->reserve(size);
			};
		auto addMultipart = [](const char* name, const char* fileName, const char* contentType, const char* data, size_t index, void* additionalData)
			{
				reinterpret_cast<vector<web::Multipart>*>(additionalData)->emplace_back
				(
					name,
					fileName,
					contentType,
					data
				);
			};

		implementation->getMultiparts(initMultipartsBuffer, addMultipart, &multiparts);
	}

	void HTTPRequestExecutors::initChunks()
	{
		auto addChunk = [](const char* chunk, size_t chunkSize, size_t index, void* buffer)
			{
				reinterpret_cast<vector<string>*>(buffer)->emplace_back(chunk, chunkSize);
			};
		auto initChunkBuffer = [](size_t size, void* buffer)
			{
				reinterpret_cast<vector<string>*>(buffer)->reserve(size);
			};

		implementation->getChunks(initChunkBuffer, addChunk, &chunks);
	}

	interfaces::IHTTPRequest* HTTPRequestExecutors::getImplementation() const
	{
		return implementation;
	}

	vector<interfaces::CVariable> HTTPRequestExecutors::convertVariables(const unordered_map<string, string>& variables)
	{
		vector<interfaces::CVariable> result;

		result.reserve(variables.size());

		for (const auto& [key, value] : variables)
		{
			result.emplace_back(key.data(), value.data());
		}

		return result;
	}

	HTTPRequestExecutors::HTTPRequestExecutors(interfaces::IHTTPRequest* implementation, const function<void(interfaces::IHTTPRequest*)>& deleter) :
		implementation(implementation),
		deleter(deleter)
	{
		json.setJSONData(string_view(implementation->getJSON()));

		this->initHeaders();
		this->initQueryParameters();
		this->initMultiparts();
		this->initChunks();
	}

	HTTPRequestExecutors::HTTPRequestExecutors(HTTPRequestExecutors&& other) noexcept
	{
		(*this) = move(other);
	}

	HTTPRequestExecutors& HTTPRequestExecutors::operator =(HTTPRequestExecutors&& other) noexcept
	{
		implementation = other.implementation;
		deleter = other.deleter;
		json = move(other.json);
		headers = move(other.headers);
		queryParameters = move(other.queryParameters);
		multiparts = move(other.multiparts);
		chunks = move(other.chunks);

		other.implementation = nullptr;
		other.deleter = nullptr;

		return *this;
	}

	void HTTPRequestExecutors::updateLargeData(string_view dataPart, size_t size)
	{
		implementation->updateLargeData(dataPart.data(), dataPart.size(), size);
	}

	string_view HTTPRequestExecutors::getRawParameters() const
	{
		return implementation->getRawParameters();
	}

	string_view HTTPRequestExecutors::getMethod() const
	{
		return implementation->getMethod();
	}

	const unordered_map<string, string>& HTTPRequestExecutors::getQueryParameters() const
	{
		return queryParameters;
	}

	string HTTPRequestExecutors::getHTTPVersion() const
	{
		return "HTTP/" + to_string(implementation->getHTTPVersion());
	}

	const web::HeadersMap& HTTPRequestExecutors::getHeaders() const
	{
		return headers;
	}

	string_view HTTPRequestExecutors::getBody() const
	{
		return implementation->getBody();
	}

	void HTTPRequestExecutors::setAttribute(string_view name, string_view value)
	{
		implementation->setAttribute(name.data(), value.data());
	}

	string HTTPRequestExecutors::getAttribute(string_view name)
	{
		const char* temp = implementation->getAttribute(name.data());
		string result(temp);

		implementation->deleteAttribute(temp);

		return result;
	}

	void HTTPRequestExecutors::deleteSession()
	{
		implementation->deleteSession();
	}

	void HTTPRequestExecutors::removeAttribute(string_view name)
	{
		implementation->removeAttribute(name.data());
	}

	web::HeadersMap HTTPRequestExecutors::getCookies() const
	{
		web::HeadersMap result;
		auto initCookiesBuffer = [](size_t size, void* buffer)
			{
				reinterpret_cast<web::HeadersMap*>(buffer)->reserve(size);
			};
		auto addCookie = [](const char* key, const char* value, size_t index, void* buffer)
			{
				reinterpret_cast<web::HeadersMap*>(buffer)->try_emplace(key, value);
			};

		implementation->getCookies(initCookiesBuffer, addCookie, &result);

		return result;
	}

	const vector<web::Multipart>& HTTPRequestExecutors::getMultiparts() const
	{
		return multiparts;
	}

	LargeData HTTPRequestExecutors::getLargeData() const
	{
		const interfaces::CLargeData* data = implementation->getLargeData();

		return LargeData(string_view(data->dataPart, data->dataPartSize), data->bodySize, data->isLastPacket);
	}

	void HTTPRequestExecutors::sendAssetFile(string_view filePath, HTTPResponseExecutors& response, const unordered_map<string, string>& variables, bool isBinary, string_view fileName)
	{
		vector<interfaces::CVariable> temp = HTTPRequestExecutors::convertVariables(variables);

		implementation->sendAssetFile(filePath.data(), response.implementation, temp.size(), temp.data(), isBinary, fileName.data());
	}

	void HTTPRequestExecutors::sendStaticFile(string_view filePath, HTTPResponseExecutors& response, bool isBinary, string_view fileName)
	{
		implementation->sendStaticFile(filePath.data(), response.implementation, isBinary, fileName.data());
	}

	void HTTPRequestExecutors::sendDynamicFile(string_view filePath, HTTPResponseExecutors& response, const unordered_map<string, string>& variables, bool isBinary, string_view fileName)
	{
		vector<interfaces::CVariable> temp = HTTPRequestExecutors::convertVariables(variables);

		implementation->sendWFDPFile(filePath.data(), response.implementation, temp.size(), temp.data(), isBinary, fileName.data());
	}

	void HTTPRequestExecutors::streamFile(string_view filePath, HTTPResponseExecutors& response, string_view fileName, size_t chunkSize)
	{
		implementation->streamFile(filePath.data(), response.implementation, fileName.data(), chunkSize);
	}

	void HTTPRequestExecutors::registerWFDPFunction(string_view functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result))
	{
		implementation->registerWFDPFunction(functionName.data(), function, deleter);
	}

	void HTTPRequestExecutors::unregisterWFDPFunction(string_view functionName)
	{
		implementation->unregisterWFDPFunction(functionName.data());
	}

	bool HTTPRequestExecutors::isWFDPFunctionRegistered(string_view functionName)
	{
		return implementation->isWFDPFunctionRegistered(functionName.data());
	}

	const json::JSONParser& HTTPRequestExecutors::getJSON() const
	{
		return json;
	}

	const vector<string>& HTTPRequestExecutors::getChunks() const
	{
		return chunks;
	}

	string_view HTTPRequestExecutors::getRawRequest() const
	{
		return implementation->getRawRequest();
	}

	string HTTPRequestExecutors::getClientIpV4() const
	{
		const char* temp = implementation->getClientIpV4();
		string result(temp);

		implementation->deleteClientIpV4(temp);

		return result;
	}

	string HTTPRequestExecutors::getServerIpV4() const
	{
		const char* temp = implementation->getServerIpV4();
		string result(temp);

		implementation->deleteServerIpV4(temp);

		return result;
	}

	uint16_t HTTPRequestExecutors::getClientPort() const
	{
		return implementation->getClientPort();
	}

	uint16_t HTTPRequestExecutors::getServerPort() const
	{
		return implementation->getServerPort();
	}

	DatabaseExecutors HTTPRequestExecutors::getOrCreateDatabase(string_view databaseName)
	{
		return DatabaseExecutors(implementation->getOrCreateDatabase(databaseName.data()));
	}

	DatabaseExecutors HTTPRequestExecutors::getDatabase(string_view databaseName) const
	{
		return DatabaseExecutors(implementation->getDatabase(databaseName.data()));
	}

	TableExecutors HTTPRequestExecutors::getOrCreateTable(string_view databaseName, string_view tableName, string_view createTableQuery)
	{
		return this->getOrCreateDatabase(databaseName).getOrCreateTable(tableName, createTableQuery);
	}

	TableExecutors HTTPRequestExecutors::getTable(string_view databaseName, string_view tableName) const
	{
		return this->getDatabase(databaseName).getTable(tableName);
	}

	HTTPRequestExecutors::~HTTPRequestExecutors()
	{
		if (deleter && implementation)
		{
			deleter(implementation);

			implementation = nullptr;
		}
	}
}
