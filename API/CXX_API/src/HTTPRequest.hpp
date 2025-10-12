#pragma once

#include <functional>
#include <algorithm>

#include "WebInterfaces/IHTTPRequest.h"
#include "HTTPResponse.hpp"
#include "Utility/ChunkGenerator.hpp"
#include "JSONParser.hpp"
#include "Exceptions/WebFrameworkAPIException.hpp"
#include "DLLHandler.hpp"
#include "Databases/Database.hpp"

namespace framework
{
	template<typename T>
	concept RouteParameterType = std::same_as<T, std::string> || std::integral<T> || std::floating_point<T>;

	class Multipart
	{
	private:
		std::string name;
		std::optional<std::string> fileName;
		std::optional<std::string> contentType;
		std::string data;

	public:
		Multipart(const char* name, const char* fileName, const char* contentType, const char* data);

		const std::string& getName() const;

		const std::optional<std::string>& getFileName() const;

		const std::optional<std::string>& getContentType() const;

		const std::string& getData() const;

		~Multipart() = default;
	};

	struct LargeData
	{
		std::string_view dataPart;
		bool isLastPacket;

		LargeData(std::string_view dataPart, bool isLastPacket);
	};

	class HTTPRequest
	{
	public:
		struct InsensitiveStringHash
		{
			size_t operator () (const std::string& value) const;
		};

		struct InsensitiveStringEqual
		{
			bool operator () (const std::string& left, const std::string& right) const;
		};

	public:
		using HeadersMap = std::unordered_map<std::string, std::string, InsensitiveStringHash, InsensitiveStringEqual>;

	private:
		static std::vector<interfaces::CVariable> convertVariables(const std::unordered_map<std::string, std::string>& variables);

	private:
		interfaces::IHTTPRequest* implementation;
		JSONParser json;
		HeadersMap headers;
		std::unordered_map<std::string, std::string> queryParameters;
		std::vector<Multipart> multiparts;
		std::vector<std::string> chunks;

	private:
		void initHeaders();

		void initQueryParameters();

		void initMultiparts();

		void initChunks();

	public:
		template<std::derived_from<exceptions::WebFrameworkAPIException> T>
		static bool isException(const std::exception& exception);

	public:
		interfaces::IHTTPRequest* getImplementation() const;

	public:
		HTTPRequest(interfaces::IHTTPRequest* implementation);

		HTTPRequest(const HTTPRequest&) = delete;

		HTTPRequest(HTTPRequest&& other) noexcept;

		HTTPRequest& operator =(const HTTPRequest&) = delete;

		HTTPRequest& operator =(HTTPRequest&& other) noexcept;

		/// <summary>
		/// Parameters string from HTTP
		/// </summary>
		/// <returns>HTTP parameters</returns>
		std::string_view getRawParameters() const;

		/// <summary>
		/// HTTP request method
		/// </summary>
		/// <returns>HTTP method</returns>
		std::string_view getMethod() const;

		/// <summary>
		/// GET parameters
		/// </summary>
		/// <returns>GET parameters as map</returns>
		const std::unordered_map<std::string, std::string>& getQueryParameters() const;

		/// <summary>
		/// HTTP version
		/// </summary>
		/// <returns>HTTP version</returns>
		std::string getHTTPVersion() const;

		/// <summary>
		/// All HTTP headers
		/// </summary>
		/// <returns>HTTP headers as map</returns>
		const HeadersMap& getHeaders() const;

		/// <summary>
		/// HTTP request body
		/// </summary>
		/// <returns>HTTP request body</returns>
		std::string_view getBody() const;

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name">attribute name</param>
		/// <param name="value">attribute value</param>
		void setAttribute(std::string_view name, std::string_view value);

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name">attribute name</param>
		/// <returns>attribute value</returns>
		/// <exception cref="std::out_of_range"></exception>
		std::string getAttribute(std::string_view name);

		/// <summary>
		/// Session wrapper
		/// </summary>
		void deleteSession();

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name"></param>
		void removeAttribute(std::string_view name);

		/// <summary>
		/// Client's cookies
		/// </summary>
		/// <returns>HTTP cookies as map</returns>
		HeadersMap getCookies() const;

		/**
		 * @brief Get data from multipart/form-data
		 * @return
		 */
		const std::vector<Multipart>& getMultiparts() const;

		LargeData getLargeData() const;

		/// <summary>
		/// ResourceExecutor wrapper
		/// </summary>
		/// <param name="filePath">path to asset file from assets folder</param>
		/// <param name="fileName">Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required</param>
		/// <exception cref="framework::exceptions::DynamicPagesSyntaxException"></exception>
		/// <exception cref="std::exception"></exception>
		void sendAssetFile(std::string_view filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables = {}, bool isBinary = true, std::string_view fileName = "");

		/**
		* Send non dynamic file
		* @param filePath Path to asset file from assets folder
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required
		* @exception std::exception
		*/
		void sendStaticFile(std::string_view filePath, HTTPResponse& response, bool isBinary = true, std::string_view fileName = "");

		/**
		* Send .wfdp file
		* @param filePath Path to asset file from assets folder
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required
		* @exception framework::exceptions::DynamicPagesSyntaxException
		* @exception std::exception
		*/
		void sendWFDPFile(std::string_view filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary = false, std::string_view fileName = "");

		/**
		* Send large files
		* @param filePath Path to asset file from assets folder
		* @param fileName Name of file in Content-Disposition HTTP header, ASCII name required
		* @param chunkSize Desired size of read data before sending
		*/
		void streamFile(std::string_view filePath, HTTPResponse& response, std::string_view fileName, size_t chunkSize = interfaces::IHTTPRequest::defaultChunkSize);

		/// @brief Add new function in .wfdp interpreter
		/// @param functionName Name of new function
		/// @param function Function implementation
		void registerWFDPFunction(std::string_view functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result));

		/// @brief Remove function from .wfdp interpreter
		/// @param functionName Name of function
		void unregisterWFDPFunction(std::string_view functionName);

		/// @brief Check if function is registered
		/// @param functionName Name of function
		/// @return true if function is registered, false otherwise
		bool isWFDPFunctionRegistered(std::string_view functionName);

		/**
		 * @brief Get file content
		 * @param filePath Path to asset file from assets folder
		 * @return 
		 */
		std::string getFile(const std::filesystem::path& filePath) const;

		/**
		 * @brief Process static files like .md
		 * @param fileData Static file content
		 * @param fileExtension Supported processing extension
		 * @return 
		 */
		std::string processStaticFile(std::string_view fileData, std::string_view fileExtension);

		/**
		 * @brief Process .wfdp files
		 * @param fileData WFDP file content
		 * @param variables 
		 * @return 
		 */
		std::string processWFDPFile(std::string_view fileData, const std::unordered_map<std::string, std::string>& variables);

		/// <summary>
		/// Getter for JSONParser
		/// </summary>
		/// <returns>JSONParser</returns>
		const JSONParser& getJSON() const;

		/**
		 * @brief Get chunks
		 * @return
		 */
		const std::vector<std::string>& getChunks() const;

		std::string_view getRawRequest() const;

		/// <summary>
		/// Get client's address
		/// </summary>
		/// <returns>client's address</returns>
		std::string getClientIpV4() const;

		/// <summary>
		/// Get server's address
		/// </summary>
		/// <returns>server's address</returns>
		std::string getServerIpV4() const;

		/// <summary>
		/// Get client's port
		/// </summary>
		/// <returns>client's port</returns>
		uint16_t getClientPort() const;

		/// <summary>
		/// Get server's port
		/// </summary>
		/// <returns>server's port</returns>
		uint16_t getServerPort() const;

		Database getOrCreateDatabase(std::string_view databaseName);

		Database getDatabase(std::string_view databaseName) const;

		Table getOrCreateTable(std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery);

		Table getTable(std::string_view databaseName, std::string_view tableName) const;

		template<RouteParameterType T>
		T getRouteParameter(std::string_view routeParameterName) const;

		/**
		 * @brief Send runtime generated content
		 * @tparam ...Args
		 * @tparam T
		 * @param response
		 * @param ...args
		 */
		template<std::derived_from<utility::ChunkGenerator> T, typename... Args>
		void sendChunks(HTTPResponse& response, Args&&... args);

		/**
		 * @brief Send file
		 * @tparam ...Args
		 * @tparam T
		 * @param response
		 * @param fileName
		 * @param ...args
		 */
		template<std::derived_from<utility::ChunkGenerator> T, typename... Args>
		void sendFileChunks(HTTPResponse& response, std::string_view fileName, Args&&... args);

		template<std::derived_from<exceptions::WebFrameworkAPIException> T = exceptions::WebFrameworkAPIException, typename... Args>
		void throwException(Args&&... args);

		~HTTPRequest();

		friend class ExecutorsManager;
	};

	template<RouteParameterType T>
	T HTTPRequest::getRouteParameter(std::string_view routeParameterName) const
	{
		if constexpr (std::is_same_v<T, std::string>)
		{
			std::string result(implementation->getRouteStringParameter(routeParameterName.data()));

			return result;
		}
		else if constexpr (std::is_integral_v<T>)
		{
			return static_cast<T>(implementation->getRouteIntegerParameter(routeParameterName.data()));
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			return static_cast<T>(implementation->getRouteDoubleParameter(routeParameterName.data()));
		}
		else
		{
			throw std::runtime_error(std::format("Wrong route parameter type: {}", typeid(T).name()));

			return T();
		}
	}

	template<std::derived_from<utility::ChunkGenerator> T, typename... Args>
	inline void HTTPRequest::sendChunks(HTTPResponse& response, Args&&... args)
	{
		this->sendFileChunks<T>(response, "", std::forward<Args>(args)...);
	}

	template<std::derived_from<utility::ChunkGenerator> T, typename... Args>
	inline void HTTPRequest::sendFileChunks(HTTPResponse& response, std::string_view fileName, Args&&... args)
	{
		T generator(std::forward<Args>(args)...);

		implementation->sendFileChunks(response.implementation, fileName.data(), &generator, [](void* chunkGenerator) -> const char* { return reinterpret_cast<T*>(chunkGenerator)->generate().data(); });
	}

	template<std::derived_from<exceptions::WebFrameworkAPIException> T, typename... Args>
	inline void HTTPRequest::throwException(Args&&... args)
	{
		T exception(std::forward<Args>(args)...);

		implementation->throwException(exception.what(), static_cast<int64_t>(exception.getResponseCode()), exception.getLogCategory().data(), typeid(T).hash_code());
	}
}

namespace framework
{
	inline size_t HTTPRequest::InsensitiveStringHash::operator() (const std::string& value) const
	{
		std::string temp;

		temp.reserve(value.size());

		std::transform
		(
			value.begin(),
			value.end(),
			std::back_inserter(temp),
			[](char c) -> char
			{
				return std::tolower(c);
			}
		);

		return std::hash<std::string>()(temp);
	}

	inline bool HTTPRequest::InsensitiveStringEqual::operator () (const std::string& left, const std::string& right) const
	{
		return std::equal
		(
			left.begin(), left.end(),
			right.begin(), right.end(),
			[](char first, char second) { return std::tolower(first) == std::tolower(second); }
		);
	}

	inline Multipart::Multipart(const char* name, const char* fileName, const char* contentType, const char* data) :
		name(name),
		data(data)
	{
		if (fileName)
		{
			this->fileName = fileName;
		}

		if (contentType)
		{
			this->contentType = contentType;
		}
	}

	inline const std::string& Multipart::getName() const
	{
		return name;
	}

	inline const std::optional<std::string>& Multipart::getFileName() const
	{
		return fileName;
	}

	inline const std::optional<std::string>& Multipart::getContentType() const
	{
		return contentType;
	}

	inline const std::string& Multipart::getData() const
	{
		return data;
	}

	inline LargeData::LargeData(std::string_view dataPart, bool isLastPacket) :
		dataPart(dataPart),
		isLastPacket(isLastPacket)
	{

	}

	inline void HTTPRequest::initHeaders()
	{
		auto initHeadersBuffer = [](size_t size, void* buffer)
			{
				reinterpret_cast<HTTPRequest::HeadersMap*>(buffer)->reserve(size);
			};
		auto addHeader = [](const char* key, const char* value, size_t index, void* buffer)
			{
				reinterpret_cast<HTTPRequest::HeadersMap*>(buffer)->try_emplace(key, value);
			};

		implementation->getHeaders(initHeadersBuffer, addHeader, &headers);
	}

	inline void HTTPRequest::initQueryParameters()
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

	inline void HTTPRequest::initMultiparts()
	{
		auto initMultipartsBuffer = [](size_t size, void* buffer)
			{
				static_cast<std::vector<Multipart>*>(buffer)->reserve(size);
			};
		auto addMultipart = [](const char* name, const char* fileName, const char* contentType, const char* data, size_t index, void* buffer)
			{
				static_cast<std::vector<Multipart>*>(buffer)->emplace_back(name, fileName, contentType, data);
			};

		implementation->getMultiparts(initMultipartsBuffer, addMultipart, &multiparts);
	}

	inline void HTTPRequest::initChunks()
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

	template<std::derived_from<exceptions::WebFrameworkAPIException> T>
	inline bool HTTPRequest::isException(const std::exception& exception)
	{
		using checkExceptionHash = bool (*)(const void* exception, size_t hash);

		return utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(checkExceptionHash, &exception, typeid(T).hash_code());
	}

	inline interfaces::IHTTPRequest* HTTPRequest::getImplementation() const
	{
		return implementation;
	}

	inline std::vector<interfaces::CVariable> HTTPRequest::convertVariables(const std::unordered_map<std::string, std::string>& variables)
	{
		std::vector<interfaces::CVariable> result;

		result.reserve(variables.size());

		for (const auto& [key, value] : variables)
		{
			result.emplace_back(key.data(), value.data());
		}

		return result;
	}

	inline HTTPRequest::HTTPRequest(interfaces::IHTTPRequest* implementation) :
		implementation(implementation),
		json(implementation->getJSON())
	{
		this->initHeaders();
		this->initQueryParameters();
		this->initMultiparts();
		this->initChunks();
	}

	inline HTTPRequest::HTTPRequest(HTTPRequest&& other) noexcept
	{
		(*this) = std::move(other);
	}

	inline HTTPRequest& HTTPRequest::operator =(HTTPRequest&& other) noexcept
	{
		implementation = other.implementation;
		json = std::move(other.json);
		headers = move(other.headers);
		queryParameters = move(other.queryParameters);
		multiparts = move(other.multiparts);
		chunks = move(other.chunks);

		other.implementation = nullptr;

		return *this;
	}

	inline std::string_view HTTPRequest::getRawParameters() const
	{
		return implementation->getRawParameters();
	}

	inline std::string_view HTTPRequest::getMethod() const
	{
		return implementation->getMethod();
	}

	inline const std::unordered_map<std::string, std::string>& HTTPRequest::getQueryParameters() const
	{
		return queryParameters;
	}

	inline std::string HTTPRequest::getHTTPVersion() const
	{
		return std::format("HTTP/{}", implementation->getHTTPVersion());
	}

	inline const HTTPRequest::HeadersMap& HTTPRequest::getHeaders() const
	{
		return headers;
	}

	inline std::string_view HTTPRequest::getBody() const
	{
		size_t bodySize = 0;
		const char* result = implementation->getBody(&bodySize);

		return std::string_view(result, bodySize);
	}

	inline void HTTPRequest::setAttribute(std::string_view name, std::string_view value)
	{
		implementation->setAttribute(name.data(), value.data());
	}

	inline std::string HTTPRequest::getAttribute(std::string_view name)
	{
		const char* temp = implementation->getAttribute(name.data());
		std::string result(temp);

		implementation->deleteAttribute(temp);

		return result;
	}

	inline void HTTPRequest::deleteSession()
	{
		implementation->deleteSession();
	}

	inline void HTTPRequest::removeAttribute(std::string_view name)
	{
		implementation->removeAttribute(name.data());
	}

	inline HTTPRequest::HeadersMap HTTPRequest::getCookies() const
	{
		HeadersMap result;
		auto initCookiesBuffer = [](size_t size, void* buffer)
			{
				reinterpret_cast<HTTPRequest::HeadersMap*>(buffer)->reserve(size);
			};
		auto addCookie = [](const char* key, const char* value, size_t index, void* buffer)
			{
				reinterpret_cast<HTTPRequest::HeadersMap*>(buffer)->try_emplace(key, value);
			};

		implementation->getCookies(initCookiesBuffer, addCookie, &result);

		return result;
	}

	inline const std::vector<Multipart>& HTTPRequest::getMultiparts() const
	{
		return multiparts;
	}

	inline LargeData HTTPRequest::getLargeData() const
	{
		const interfaces::CLargeData* data = implementation->getLargeData();

		return LargeData(std::string_view(data->dataPart, data->dataPartSize), data->isLastPacket);
	}

	inline void HTTPRequest::sendAssetFile(std::string_view filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary, std::string_view fileName)
	{
		std::vector<interfaces::CVariable> temp = HTTPRequest::convertVariables(variables);

		implementation->sendAssetFile(filePath.data(), response.implementation, temp.size(), temp.data(), isBinary, fileName.data());
	}

	inline void HTTPRequest::sendStaticFile(std::string_view filePath, HTTPResponse& response, bool isBinary, std::string_view fileName)
	{
		implementation->sendStaticFile(filePath.data(), response.implementation, isBinary, fileName.data());
	}

	inline void HTTPRequest::sendWFDPFile(std::string_view filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary, std::string_view fileName)
	{
		std::vector<interfaces::CVariable> temp = HTTPRequest::convertVariables(variables);

		implementation->sendWFDPFile(filePath.data(), response.implementation, temp.size(), temp.data(), isBinary, fileName.data());
	}

	inline void HTTPRequest::streamFile(std::string_view filePath, HTTPResponse& response, std::string_view fileName, size_t chunkSize)
	{
		implementation->streamFile(filePath.data(), response.implementation, fileName.data(), chunkSize);
	}

	inline void HTTPRequest::registerWFDPFunction(std::string_view functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result))
	{
		implementation->registerWFDPFunction(functionName.data(), function, deleter);
	}

	inline void HTTPRequest::unregisterWFDPFunction(std::string_view functionName)
	{
		implementation->unregisterWFDPFunction(functionName.data());
	}

	inline bool HTTPRequest::isWFDPFunctionRegistered(std::string_view functionName)
	{
		return implementation->isWFDPFunctionRegistered(functionName.data());
	}

	inline std::string HTTPRequest::getFile(const std::filesystem::path& filePath) const
	{
		std::string result;
		auto fillBuffer = [](const char* data, size_t size, void* buffer)
			{
				static_cast<std::string*>(buffer)->append(data, size);
			};

		implementation->getFile(filePath.string().data(), fillBuffer, &result);

		return result;
	}

	inline std::string HTTPRequest::processStaticFile(std::string_view fileData, std::string_view fileExtension)
	{
		std::string result;
		auto fillBuffer = [](const char* data, size_t size, void* buffer)
			{
				static_cast<std::string*>(buffer)->append(data, size);
			};

		implementation->processStaticFile(fileData.data(), fileData.size(), fileExtension.data(), fillBuffer, &result);

		return result;
	}

	inline std::string HTTPRequest::processWFDPFile(std::string_view fileData, const std::unordered_map<std::string, std::string>& variables)
	{
		std::string result;
		std::vector<interfaces::CVariable> temp = HTTPRequest::convertVariables(variables);
		auto fillBuffer = [](const char* data, size_t size, void* buffer)
			{
				static_cast<std::string*>(buffer)->append(data, size);
			};

		implementation->processWFDPFile(fileData.data(), fileData.size(), temp.data(), temp.size(), fillBuffer, &result);

		return result;
	}

	inline const JSONParser& HTTPRequest::getJSON() const
	{
		return json;
	}

	inline const std::vector<std::string>& HTTPRequest::getChunks() const
	{
		return chunks;
	}

	inline std::string_view HTTPRequest::getRawRequest() const
	{
		return implementation->getRawRequest();
	}

	inline std::string HTTPRequest::getClientIpV4() const
	{
		const char* temp = implementation->getClientIpV4();
		std::string result(temp);

		implementation->deleteClientIpV4(temp);

		return result;
	}

	inline std::string HTTPRequest::getServerIpV4() const
	{
		const char* temp = implementation->getServerIpV4();
		std::string result(temp);

		implementation->deleteServerIpV4(temp);

		return result;
	}

	inline uint16_t HTTPRequest::getClientPort() const
	{
		return implementation->getClientPort();
	}

	inline uint16_t HTTPRequest::getServerPort() const
	{
		return implementation->getServerPort();
	}

	inline Database HTTPRequest::getOrCreateDatabase(std::string_view databaseName)
	{
		return Database(implementation->getOrCreateDatabase(databaseName.data()));
	}

	inline Database HTTPRequest::getDatabase(std::string_view databaseName) const
	{
		return Database(implementation->getDatabase(databaseName.data()));
	}

	inline Table HTTPRequest::getOrCreateTable(std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery)
	{
		return this->getOrCreateDatabase(databaseName).getOrCreateTable(tableName, createTableQuery);
	}

	inline Table HTTPRequest::getTable(std::string_view databaseName, std::string_view tableName) const
	{
		return this->getDatabase(databaseName).getTable(tableName);
	}

	inline HTTPRequest::~HTTPRequest()
	{
		implementation = nullptr;
	}
}
