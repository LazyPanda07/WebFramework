#include "HTTPRequest.h"

using namespace std;

static void addHeader(const char* key, const char* value, void* additionalData);

static void addKeyValue(const char* key, const char* value, void* additionalData);

static void addCookie(const char* key, const char* value, void* additionalData);

static void addMultipart(const framework::interfaces::CMultipart* multipart, void* additionalData);

static void addChunk(const char* chunk, size_t chunkSize, void* additionalData);

namespace framework
{
	LargeData::LargeData(string_view dataPart, size_t size, bool isLastPacket) :
		dataPart(dataPart),
		size(size),
		isLastPacket(isLastPacket)
	{

	}

	void HTTPRequest::initHeaders()
	{
		implementation->getHeaders(addHeader, &headers);
	}

	void HTTPRequest::initKeyValuesParameters()
	{
		implementation->getKeyValueParameters(addKeyValue, &keyValueParameters);
	}

	void HTTPRequest::initMultiparts()
	{
		implementation->getMultiparts(addMultipart, &multiparts);
	}

	void HTTPRequest::initChunks()
	{
		implementation->getChunks(addChunk, &chunks);
	}

	interfaces::IHTTPRequest* HTTPRequest::getImplementation() const
	{
		return implementation;
	}

	vector<interfaces::CVariable> HTTPRequest::convertVariables(const std::unordered_map<std::string, std::string>& variables)
	{
		vector<interfaces::CVariable> result;

		result.reserve(variables.size());

		for (const auto& [key, value] : variables)
		{
			result.emplace_back(key.data(), value.data());
		}

		return result;
	}

	HTTPRequest::HTTPRequest(interfaces::IHTTPRequest* implementation, const function<void(interfaces::IHTTPRequest*)>& deleter) :
		implementation(implementation),
		deleter(deleter)
	{
		json.setJSONData(string_view(implementation->getJSON()));

		this->initHeaders();
		this->initKeyValuesParameters();
		this->initMultiparts();
		this->initChunks();
	}

	void HTTPRequest::updateLargeData(string_view dataPart, size_t size)
	{
		implementation->updateLargeData(dataPart.data(), dataPart.size(), size);
	}

	string_view HTTPRequest::getRawParameters() const
	{
		return implementation->getRawParameters();
	}

	string_view HTTPRequest::getMethod() const
	{
		return implementation->getMethod();
	}

	const unordered_map<string, string>& HTTPRequest::getKeyValueParameters() const
	{
		return keyValueParameters;
	}

	string HTTPRequest::getHTTPVersion() const
	{
		return "HTTP/" + to_string(implementation->getHTTPVersion());
	}

	const web::HeadersMap& HTTPRequest::getHeaders() const
	{
		return headers;
	}

	string_view HTTPRequest::getBody() const
	{
		return implementation->getBody();
	}

	void HTTPRequest::setAttribute(string_view name, string_view value)
	{
		implementation->setAttribute(name.data(), value.data());
	}

	string HTTPRequest::getAttribute(string_view name)
	{
		const char* temp = implementation->getAttribute(name.data());
		string result(temp);

		implementation->deleteAttribute(temp);

		return result;
	}

	void HTTPRequest::deleteSession()
	{
		implementation->deleteSession();
	}

	void HTTPRequest::removeAttribute(string_view name)
	{
		implementation->removeAttribute(name.data());
	}

	web::HeadersMap HTTPRequest::getCookies() const
	{
		web::HeadersMap result;

		implementation->getCookies(addCookie, &result);

		return result;
	}

	const vector<web::Multipart>& HTTPRequest::getMultiparts() const
	{
		return multiparts;
	}

	LargeData HTTPRequest::getLargeData() const
	{
		const interfaces::CLargeData* data = implementation->getLargeData();

		return LargeData(string_view(data->dataPart, data->dataPartSize), data->bodySize, data->isLastPacket);
	}

	void HTTPRequest::sendAssetFile(string_view filePath, HTTPResponse& response, const unordered_map<string, string>& variables, bool isBinary, string_view fileName)
	{
		vector<interfaces::CVariable> temp = HTTPRequest::convertVariables(variables);

		implementation->sendAssetFile(filePath.data(), response.implementation, temp.size(), temp.data(), isBinary, fileName.data());
	}

	void HTTPRequest::sendStaticFile(string_view filePath, HTTPResponse& response, bool isBinary, string_view fileName)
	{
		implementation->sendStaticFile(filePath.data(), response.implementation, isBinary, fileName.data());
	}

	void HTTPRequest::sendDynamicFile(string_view filePath, HTTPResponse& response, const unordered_map<string, string>& variables, bool isBinary, string_view fileName)
	{
		vector<interfaces::CVariable> temp = HTTPRequest::convertVariables(variables);

		implementation->sendDynamicFile(filePath.data(), response.implementation, temp.size(), temp.data(), isBinary, fileName.data());
	}

	void HTTPRequest::streamFile(string_view filePath, HTTPResponse& response, string_view fileName, size_t chunkSize)
	{
		implementation->streamFile(filePath.data(), response.implementation, fileName.data(), chunkSize);
	}

	void HTTPRequest::registerDynamicFunction(string_view functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result))
	{
		implementation->registerDynamicFunction(functionName.data(), function, deleter);
	}

	void HTTPRequest::unregisterDynamicFunction(string_view functionName)
	{
		implementation->unregisterDynamicFunction(functionName.data());
	}

	bool HTTPRequest::isDynamicFunctionRegistered(string_view functionName)
	{
		return implementation->isDynamicFunctionRegistered(functionName.data());
	}

	const json::JSONParser& HTTPRequest::getJSON() const
	{
		return json;
	}

	const vector<string>& HTTPRequest::getChunks() const
	{
		return chunks;
	}

	string_view HTTPRequest::getRawRequest() const
	{
		return implementation->getRawRequest();
	}

	string HTTPRequest::getClientIpV4() const
	{
		const char* temp = implementation->getClientIpV4();
		string result(temp);

		implementation->deleteClientIpV4(temp);

		return result;
	}

	string HTTPRequest::getServerIpV4() const
	{
		const char* temp = implementation->getServerIpV4();
		string result(temp);

		implementation->deleteServerIpV4(temp);

		return result;
	}

	uint16_t HTTPRequest::getClientPort() const
	{
		return implementation->getClientPort();
	}

	uint16_t HTTPRequest::getServerPort() const
	{
		return implementation->getServerPort();
	}

	HTTPRequest::~HTTPRequest()
	{
		if (deleter && implementation)
		{
			deleter(implementation);

			implementation = nullptr;
		}
	}
}

void addHeader(const char* key, const char* value, void* additionalData)
{
	reinterpret_cast<web::HeadersMap*>(additionalData)->try_emplace(key, value);
}

void addKeyValue(const char* key, const char* value, void* additionalData)
{
	reinterpret_cast<unordered_map<string, string>*>(additionalData)->try_emplace(key, value);
}

void addCookie(const char* key, const char* value, void* additionalData)
{
	reinterpret_cast<web::HeadersMap*>(additionalData)->try_emplace(key, value);
}

void addMultipart(const framework::interfaces::CMultipart* multipart, void* additionalData)
{
	optional<string> fileName;
	optional<string> contentType;

	if (multipart->fileName)
	{
		fileName = multipart->fileName;
	}

	if (multipart->contentType)
	{
		contentType = multipart->contentType;
	}

	reinterpret_cast<vector<web::Multipart>*>(additionalData)->emplace_back
	(
		multipart->name,
		fileName,
		contentType,
		multipart->data
	);
}

void addChunk(const char* chunk, size_t chunkSize, void* additionalData)
{
	reinterpret_cast<vector<string>*>(additionalData)->emplace_back(chunk, chunkSize);
}
