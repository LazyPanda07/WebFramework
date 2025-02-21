#pragma once

#include "Import/WebFrameworkCore.h"

#include "BaseTCPServer.h"
#include "HTTPParser.h"
#include "MultiLocalizationManager.h"
#include "Log.h"

#include "Managers/SessionsManager.h"
#include "SQLite3/SQLiteManager.h"
#include "Interfaces/IStaticFile.h"
#include "Interfaces/IDynamicFile.h"
#include "Utility/ChunkGenerator.h"
#include "Utility/WebFrameworkConcepts.h"

namespace framework
{
	/// <summary>
	/// Parsing HTTP request
	/// <para>Accessing to sessions</para>
	/// <para>Overriding input stream operator for simplify HTTP request initializing</para>
	/// </summary>
	class WEB_FRAMEWORK_API HTTPRequest
	{
	private:
		SessionsManager& session;
		const web::BaseTCPServer& serverReference;
		streams::IOSocketStream& stream;
		sqlite::SQLiteManager& database;
		std::unordered_map<std::string, std::variant<std::string, int64_t, double>> routeParameters;
		sockaddr clientAddr;
		web::HTTPParser parser;
		interfaces::IStaticFile& staticResources;
		interfaces::IDynamicFile& dynamicResources;
		std::string_view largeDataPart;
		size_t bodySize;

	private:
		static bool isWebFrameworkDynamicPages(const std::string& filePath);

	public:
		static web::HTTPParser sendRequestToAnotherServer(std::string_view ip, std::string_view port, std::string_view request, DWORD timeout = 30'000, bool useHTTPS = false);

	public:
		HTTPRequest(SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources, sqlite::SQLiteManager& database, sockaddr clientAddr, streams::IOSocketStream& stream);

		HTTPRequest(HTTPRequest&&) noexcept = default;

		HTTPRequest(const HTTPRequest&) = default;

		HTTPRequest& operator =(HTTPRequest&&) noexcept = default;

		HTTPRequest& operator =(const HTTPRequest&) = default;

		/// <summary>
		/// Parameters string from HTTP
		/// </summary>
		/// <returns>HTTP parameters</returns>
		const std::string& getRawParameters() const;

		/// <summary>
		/// HTTP request method
		/// </summary>
		/// <returns>HTTP method</returns>
		const std::string& getMethod() const;

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

		void setLargeDataPart(std::string_view largeDataPart, size_t bodySize);

		std::string_view getLargeDataPart() const;

		size_t getBodySize() const;

		/// <summary>
		/// All HTTP headers
		/// </summary>
		/// <returns>HTTP headers as map</returns>
		const web::HeadersMap& getHeaders() const;

		/// <summary>
		/// HTTP request body
		/// </summary>
		/// <returns>HTTP request body</returns>
		const std::string& getBody() const;

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name">attribute name</param>
		/// <param name="value">attribute value</param>
		void setAttribute(const std::string& name, const std::string& value);

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name">attribute name</param>
		/// <returns>attribute value</returns>
		/// <exception cref="std::out_of_range"></exception>
		std::string getAttribute(const std::string& name);

		/// <summary>
		/// Session wrapper
		/// </summary>
		void deleteSession();

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name"></param>
		void deleteAttribute(const std::string& name);

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

		/// <summary>
		/// ResourceExecutor wrapper
		/// </summary>
		/// <param name="filePath">path to asset file from assets folder</param>
		/// <param name="fileName">Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required</param>
		/// <exception cref="framework::exceptions::DynamicPagesSyntaxException"></exception>
		/// <exception cref="std::exception"></exception>
		void sendAssetFile(const std::string& filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables = {}, bool isBinary = true, const std::string& fileName = "");

		/**
		* Send non dynamic file
		* @param filePath Path to asset file from assets folder
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required
		* @exception std::exception
		*/
		void sendStaticFile(const std::string& filePath, HTTPResponse& response, bool isBinary = true, const std::string& fileName = "");

		/**
		* Send dynamic file(.wfdp)
		* @param filePath Path to asset file from assets folder
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required
		* @exception framework::exceptions::DynamicPagesSyntaxException
		* @exception std::exception
		*/
		void sendDynamicFile(const std::string& filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary = false, const std::string& fileName = "");

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
		void registerDynamicFunction(const std::string& functionName, std::function<std::string(const std::vector<std::string>&)>&& function);

		/// @brief Remove function from .wfdp interpreter
		/// @param functionName Name of function
		void unregisterDynamicFunction(const std::string& functionName);

		/// @brief Check if function is registered
		/// @param functionName Name of function
		/// @return true if function is registered, false otherwise
		bool isDynamicFunctionRegistered(const std::string& functionName);

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

		/**
		 * @brief HTTP parser getter
		 * @return
		 */
		const web::HTTPParser& getParser() const;

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

		/**
		 * @brief Getter for route parameters
		 * @tparam T RouteParameterType type
		 * @param routeParameterName Name of route parameter
		 * @return route parameter
		 * @exception std::out_of_range Can't find route parameter with this routeParameterName
		 */
		template<utility::concepts::RouteParameterType T>
		const T& getRouteParameter(const std::string& routeParameterName);

		/**
		 * @brief First call creates model with arguments other calls returns created model
		 * @tparam ...Args
		 * @tparam T
		 * @param ...args
		 * @return
		 */
		template<std::derived_from<sqlite::SQLiteDatabaseModel> T, typename... Args>
		std::shared_ptr<T> createModel(Args&&... args);

		/**
		 * @brief First call creates model without arguments other calls returns created model
		 * @tparam T
		 * @return
		 */
		template<std::derived_from<sqlite::SQLiteDatabaseModel> T>
		std::shared_ptr<T> getModel() const;

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

		/// <summary>
		/// Reading HTTP request from network
		/// </summary>
		/// <param name="stream">special class for taking data from network</param>
		/// <param name="request">class instance</param>
		/// <returns>self for builder pattern</returns>
		/// <exception cref="web::WebException"></exception>
		friend streams::IOSocketStream& operator >> (streams::IOSocketStream& stream, HTTPRequest& request);

		/// <summary>
		/// Logging operator
		/// </summary>
		/// <param name="stream">any output source</param>
		/// <param name="request">class instance</param>
		/// <returns>self for builder pattern</returns>
		friend std::ostream& operator << (std::ostream& stream, const HTTPRequest& request);

		~HTTPRequest() = default;

		friend class ExecutorsManager;
	};

	template<std::derived_from<sqlite::SQLiteDatabaseModel> T, typename... Args>
	std::shared_ptr<T> HTTPRequest::createModel(Args&&... args)
	{
		return database.add<T>(std::forward<Args>(args)...);
	}

	template<std::derived_from<sqlite::SQLiteDatabaseModel> T>
	std::shared_ptr<T> HTTPRequest::getModel() const
	{
		std::shared_ptr<T> result = database.get<T>();

		if constexpr (std::is_default_constructible_v<T>)
		{
			if (!result)
			{
				result = database.add<T>();
			}
		}
		else if (!result)
		{
			if (Log::isValid())
			{
				Log::error("Can't get or create model in HTTPRequest::getModel<T> function where T is {}", "LogWebFrameworkModels", typeid(T).name());
			}
		}

		return result;
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

		web::HTTPBuilder builder = web::HTTPBuilder().chunk(generator.generate()).partialChunks().responseCode(web::ResponseCodes::ok).headers
		(
			"Date", HTTPResponse::getFullDate(),
			"Server", "WebFramework-Server",
			"Connection", "keep-alive"
		);

		if (fileName.size())
		{
			builder.headers
			(
				"Content-Disposition", std::format(R"(attachment; filename="{}")", fileName)
			);
		}

		stream << builder.build();

		response.setIsValid(false);

		while (true)
		{
			std::string data = generator.generate();

			stream << web::HTTPBuilder::getChunk(data);

			if (stream.eof() || data.empty())
			{
				break;
			}
		}
	}
}
