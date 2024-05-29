#pragma once

#include "core.h"

#include "BaseTCPServer.h"
#include "HTTPParser.h"
#include "MultiLocalizationManager.h"

#include "WebFrameworkManagers/SessionsManager.h"
#include "SQLite3/SQLiteManager.h"
#include "Interfaces/IStaticFile.h"
#include "Interfaces/IDynamicFile.h"

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
		std::unordered_map<std::string, std::variant<std::string, int64_t>> routeParameters;
		const sockaddr& clientAddr;
		web::HTTPParser parser;
		interfaces::IStaticFile& staticResources;
		interfaces::IDynamicFile& dynamicResources;

	private:
		static bool isWebFrameworkDynamicPages(const std::string& filePath);

	public:
		/// <summary>
		/// Send request and get response from another HTTP server
		/// </summary>
		/// <param name="ip">server's address</param>
		/// <param name="port">server's port</param>
		/// <param name="request">HTTP request</param>
		/// <returns>response from another server</returns>
		/// <exception cref="web::WebException"></exception>
		static web::HTTPParser sendRequestToAnotherServer(std::string_view ip, std::string_view port, std::string_view request, DWORD timeout = 30'000, bool useHTTPS = false);

	public:
		/// @brief Construct HTTPRequest
		/// @param session Session from MultithreadedWebServer
		/// @param serverReference Reference to server
		/// @param staticResources Static resources manager
		/// @param dynamicResources Dynamic resources manager
		/// @param database Reference to database
		/// @param clientAddr Client address
		HTTPRequest(SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources, sqlite::SQLiteManager& database, const sockaddr& clientAddr, streams::IOSocketStream& stream);

		HTTPRequest(HTTPRequest&& other) noexcept;

		HTTPRequest(const HTTPRequest& other);

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
		std::unordered_map<std::string, std::string> getCookies() const;

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
		void streamFile(const std::string& filePath, HTTPResponse& response, const std::string& fileName, size_t chunkSize = defaultChunkSize);

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

		/// <summary>
		/// Getter for route parameters
		/// </summary>
		/// <typeparam name="T">can be int64_t or std::string</typeparam>
		/// <param name="routeParameterName"><para>name of route parameter</para><para>T can be int64_t or std::string</para></param>
		/// <returns>route parameter</returns>
		/// <exception cref="std::out_of_range">can't find route parameter with this routeParameterName</exception>
		template<typename T>
		const T& getRouteParameter(const std::string& routeParameterName);

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

		template<std::derived_from<sqlite::SQLiteDatabaseModel> T, typename... Args>
		std::shared_ptr<T> createModel(Args&&... args);

		template<std::derived_from<sqlite::SQLiteDatabaseModel> T>
		std::shared_ptr<T> getModel() const;

		friend class ExecutorsManager;

		~HTTPRequest() = default;
	};

	template<std::derived_from<sqlite::SQLiteDatabaseModel> T, typename... Args>
	std::shared_ptr<T> HTTPRequest::createModel(Args&&... args)
	{
		return database.add<T>(std::forward<Args>(args)...);
	}

	template<std::derived_from<sqlite::SQLiteDatabaseModel> T>
	std::shared_ptr<T> HTTPRequest::getModel() const
	{
		return database.get<T>();
	}
}
