#include "HTTPRequestExecutors.h"

namespace framework
{
	LargeData::LargeData(std::string_view dataPart, bool isLastPacket) :
		dataPart(dataPart),
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
				reinterpret_cast<std::vector<web::Multipart>*>(additionalData)->emplace_back
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
				reinterpret_cast<std::vector<std::string>*>(buffer)->emplace_back(chunk, chunkSize);
			};
		auto initChunkBuffer = [](size_t size, void* buffer)
			{
				reinterpret_cast<std::vector<std::string>*>(buffer)->reserve(size);
			};

		implementation->getChunks(initChunkBuffer, addChunk, &chunks);
	}

	interfaces::IHTTPRequest* HTTPRequestExecutors::getImplementation() const
	{
		return implementation;
	}

	std::vector<interfaces::CVariable> HTTPRequestExecutors::convertVariables(const std::unordered_map<std::string, std::string>& variables)
	{
		std::vector<interfaces::CVariable> result;

		result.reserve(variables.size());

		for (const auto& [key, value] : variables)
		{
			result.emplace_back(key.data(), value.data());
		}

		return result;
	}

	HTTPRequestExecutors::HTTPRequestExecutors(interfaces::IHTTPRequest* implementation, const std::function<void(interfaces::IHTTPRequest*)>& deleter) :
		implementation(implementation),
		deleter(deleter)
	{
		json.setJSONData(std::string_view(implementation->getJSON()));

		this->initHeaders();
		this->initQueryParameters();
		this->initMultiparts();
		this->initChunks();
	}

	HTTPRequestExecutors::HTTPRequestExecutors(HTTPRequestExecutors&& other) noexcept
	{
		(*this) = std::move(other);
	}

	HTTPRequestExecutors& HTTPRequestExecutors::operator =(HTTPRequestExecutors&& other) noexcept
	{
		implementation = other.implementation;
		deleter = other.deleter;
		json = std::move(other.json);
		headers = move(other.headers);
		queryParameters = move(other.queryParameters);
		multiparts = move(other.multiparts);
		chunks = move(other.chunks);

		other.implementation = nullptr;
		other.deleter = nullptr;

		return *this;
	}

	void HTTPRequestExecutors::updateLargeData(std::string_view dataPart, bool isLast)
	{
		implementation->updateLargeData(dataPart.data(), dataPart.size(), isLast);
	}

	std::string_view HTTPRequestExecutors::getRawParameters() const
	{
		return implementation->getRawParameters();
	}

	std::string_view HTTPRequestExecutors::getMethod() const
	{
		return implementation->getMethod();
	}

	const std::unordered_map<std::string, std::string>& HTTPRequestExecutors::getQueryParameters() const
	{
		return queryParameters;
	}

	std::string HTTPRequestExecutors::getHTTPVersion() const
	{
		return "HTTP/" + std::to_string(implementation->getHTTPVersion());
	}

	const web::HeadersMap& HTTPRequestExecutors::getHeaders() const
	{
		return headers;
	}

	std::string_view HTTPRequestExecutors::getBody() const
	{
		return implementation->getBody();
	}

	void HTTPRequestExecutors::setAttribute(std::string_view name, std::string_view value)
	{
		implementation->setAttribute(name.data(), value.data());
	}

	std::string HTTPRequestExecutors::getAttribute(std::string_view name)
	{
		const char* temp = implementation->getAttribute(name.data());
		std::string result(temp);

		implementation->deleteAttribute(temp);

		return result;
	}

	void HTTPRequestExecutors::deleteSession()
	{
		implementation->deleteSession();
	}

	void HTTPRequestExecutors::removeAttribute(std::string_view name)
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

	const std::vector<web::Multipart>& HTTPRequestExecutors::getMultiparts() const
	{
		return multiparts;
	}

	LargeData HTTPRequestExecutors::getLargeData() const
	{
		const interfaces::CLargeData* data = implementation->getLargeData();

		return LargeData(std::string_view(data->dataPart, data->dataPartSize), data->isLastPacket);
	}

	void HTTPRequestExecutors::sendAssetFile(std::string_view filePath, HTTPResponseExecutors& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary, std::string_view fileName)
	{
		std::vector<interfaces::CVariable> temp = HTTPRequestExecutors::convertVariables(variables);

		implementation->sendAssetFile(filePath.data(), response.implementation, temp.size(), temp.data(), isBinary, fileName.data());
	}

	void HTTPRequestExecutors::sendStaticFile(std::string_view filePath, HTTPResponseExecutors& response, bool isBinary, std::string_view fileName)
	{
		implementation->sendStaticFile(filePath.data(), response.implementation, isBinary, fileName.data());
	}

	void HTTPRequestExecutors::sendDynamicFile(std::string_view filePath, HTTPResponseExecutors& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary, std::string_view fileName)
	{
		std::vector<interfaces::CVariable> temp = HTTPRequestExecutors::convertVariables(variables);

		implementation->sendWFDPFile(filePath.data(), response.implementation, temp.size(), temp.data(), isBinary, fileName.data());
	}

	void HTTPRequestExecutors::streamFile(std::string_view filePath, HTTPResponseExecutors& response, std::string_view fileName, size_t chunkSize)
	{
		implementation->streamFile(filePath.data(), response.implementation, fileName.data(), chunkSize);
	}

	void HTTPRequestExecutors::registerWFDPFunction(std::string_view functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result))
	{
		implementation->registerWFDPFunction(functionName.data(), function, deleter);
	}

	void HTTPRequestExecutors::unregisterWFDPFunction(std::string_view functionName)
	{
		implementation->unregisterWFDPFunction(functionName.data());
	}

	bool HTTPRequestExecutors::isWFDPFunctionRegistered(std::string_view functionName)
	{
		return implementation->isWFDPFunctionRegistered(functionName.data());
	}

	std::string HTTPRequestExecutors::getFile(const std::filesystem::path& filePath) const
	{
		std::string result;
		auto fillBuffer = [](const char* data, size_t size, void* buffer)
			{
				static_cast<std::string*>(buffer)->append(data, size);
			};

		implementation->getFile(filePath.string().data(), fillBuffer, &result);

		return result;
	}

	std::string HTTPRequestExecutors::processStaticFile(std::string_view fileData, std::string_view fileExtension)
	{
		std::string result;
		auto fillBuffer = [](const char* data, size_t size, void* buffer)
			{
				static_cast<std::string*>(buffer)->append(data, size);
			};

		implementation->processStaticFile(fileData.data(), fileData.size(), fileExtension.data(), fillBuffer, &result);

		return result;
	}

	std::string HTTPRequestExecutors::processWFDPFile(std::string_view fileData, const std::unordered_map<std::string, std::string>& variables)
	{
		std::string result;
		std::vector<interfaces::CVariable> temp = HTTPRequestExecutors::convertVariables(variables);
		auto fillBuffer = [](const char* data, size_t size, void* buffer)
			{
				static_cast<std::string*>(buffer)->append(data, size);
			};

		implementation->processWFDPFile(fileData.data(), fileData.size(), temp.data(), temp.size(), fillBuffer, &result);

		return result;
	}

	const json::JSONParser& HTTPRequestExecutors::getJSON() const
	{
		return json;
	}

	const std::vector<std::string>& HTTPRequestExecutors::getChunks() const
	{
		return chunks;
	}

	std::string_view HTTPRequestExecutors::getRawRequest() const
	{
		return implementation->getRawRequest();
	}

	std::string HTTPRequestExecutors::getClientIpV4() const
	{
		const char* temp = implementation->getClientIpV4();
		std::string result(temp);

		implementation->deleteClientIpV4(temp);

		return result;
	}

	std::string HTTPRequestExecutors::getServerIpV4() const
	{
		const char* temp = implementation->getServerIpV4();
		std::string result(temp);

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

	DatabaseExecutors HTTPRequestExecutors::getOrCreateDatabase(std::string_view databaseName)
	{
		return DatabaseExecutors(implementation->getOrCreateDatabase(databaseName.data()));
	}

	DatabaseExecutors HTTPRequestExecutors::getDatabase(std::string_view databaseName) const
	{
		return DatabaseExecutors(implementation->getDatabase(databaseName.data()));
	}

	TableExecutors HTTPRequestExecutors::getOrCreateTable(std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery)
	{
		return this->getOrCreateDatabase(databaseName).getOrCreateTable(tableName, createTableQuery);
	}

	TableExecutors HTTPRequestExecutors::getTable(std::string_view databaseName, std::string_view tableName) const
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
