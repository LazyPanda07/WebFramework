#pragma once

#include <functional>

#include "WebNetwork/Interfaces/IHTTPRequest.h"
#include "HTTPResponse.h"
#include "JSONParser.h"
#include "HTTPParser.h"
#include "ExecutorsConstants.h"
#include "Utility/ChunkGenerator.h"
#include "Databases/Database.h"

namespace framework
{
	template<typename T>
	concept RouteParameterType = std::same_as<T, std::string> || std::integral<T> || std::floating_point<T>;

	struct EXECUTORS_API LargeData
	{
		std::string_view dataPart;
		size_t size;
		bool isLastPacket;

		LargeData(std::string_view dataPart, size_t size, bool isLastPacket);
	};

	class EXECUTORS_API HTTPRequest
	{
	private:
		static std::vector<interfaces::CVariable> convertVariables(const std::unordered_map<std::string, std::string>& variables);

	private:
		interfaces::IHTTPRequest* implementation;
		std::function<void(interfaces::IHTTPRequest*)> deleter;
		json::JSONParser json;
		web::HeadersMap headers;
		std::unordered_map<std::string, std::string> keyValueParameters;
		std::vector<web::Multipart> multiparts;
		std::vector<std::string> chunks;

	private:
		void initHeaders();

		void initKeyValuesParameters();

		void initMultiparts();

		void initChunks();

	private:
		interfaces::IHTTPRequest* getImplementation() const;

	public:
		HTTPRequest(interfaces::IHTTPRequest* implementation, const std::function<void(interfaces::IHTTPRequest*)>& deleter = nullptr);

		HTTPRequest(const HTTPRequest&) = delete;

		HTTPRequest(HTTPRequest&& other) noexcept;

		HTTPRequest& operator =(const HTTPRequest&) = delete;

		HTTPRequest& operator =(HTTPRequest&& other) noexcept;

		void updateLargeData(std::string_view dataPart, size_t size);

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
		const std::unordered_map<std::string, std::string>& getKeyValueParameters() const;

		/// <summary>
		/// HTTP version
		/// </summary>
		/// <returns>HTTP version</returns>
		std::string getHTTPVersion() const;

		/// <summary>
		/// All HTTP headers
		/// </summary>
		/// <returns>HTTP headers as map</returns>
		const web::HeadersMap& getHeaders() const;

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
		web::HeadersMap getCookies() const;

		/**
		 * @brief Get data from multipart/form-data
		 * @return
		 */
		const std::vector<web::Multipart>& getMultiparts() const;

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
		* Send dynamic file(.wfdp)
		* @param filePath Path to asset file from assets folder
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required
		* @exception framework::exceptions::DynamicPagesSyntaxException
		* @exception std::exception
		*/
		void sendDynamicFile(std::string_view filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary = false, std::string_view fileName = "");

		/**
		* Send large files
		* @param filePath Path to asset file from assets folder
		* @param fileName Name of file in Content-Disposition HTTP header, ASCII name required
		* @param chunkSize Desired size of read data before sending
		*/
		void streamFile(std::string_view filePath, HTTPResponse& response, std::string_view fileName, size_t chunkSize = defaultChunkSize);

		/// @brief Add new function in .wfdp interpreter
		/// @param functionName Name of new function
		/// @param function Function implementation
		void registerDynamicFunction(std::string_view functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result));

		/// @brief Remove function from .wfdp interpreter
		/// @param functionName Name of function
		void unregisterDynamicFunction(std::string_view functionName);

		/// @brief Check if function is registered
		/// @param functionName Name of function
		/// @return true if function is registered, false otherwise
		bool isDynamicFunctionRegistered(std::string_view functionName);

		/// <summary>
		/// Getter for JSONParser
		/// </summary>
		/// <returns>JSONParser</returns>
		const json::JSONParser& getJSON() const;

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

		~HTTPRequest();

		friend class ExecutorsManager;
	};

	template<RouteParameterType T>
	T HTTPRequest::getRouteParameter(std::string_view routeParameterName) const
	{
		if constexpr (std::is_same_v<T, std::string>)
		{
			std::string result(implementation->getRouteParameterString(routeParameterName.data()));

			return result;
		}
		else if constexpr (std::is_integral_v<T>)
		{
			return static_cast<T>(implementation->getRouteParameterInteger(routeParameterName.data()));
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			return static_cast<T>(implementation->getRouteParameterDouble(routeParameterName.data()));
		}
		else
		{
			throw std::runtime_error(std::format("Wrong route parameter type: {}", typeid(T).name()));

			return T();
		}
	}

	template<std::derived_from<utility::ChunkGenerator> T, typename... Args>
	void HTTPRequest::sendChunks(HTTPResponse& response, Args&&... args)
	{
		this->sendFileChunks<T>(response, "", std::forward<Args>(args)...);
	}

	template<std::derived_from<utility::ChunkGenerator> T, typename... Args>
	void HTTPRequest::sendFileChunks(HTTPResponse& response, std::string_view fileName, Args&&... args)
	{
		T generator(std::forward<Args>(args)...);

		implementation->sendFileChunks(response.implementation, fileName.data(), &generator, [](void* chunkGenerator) -> const char* { return reinterpret_cast<T*>(chunkGenerator)->generate().data(); });
	}
}
