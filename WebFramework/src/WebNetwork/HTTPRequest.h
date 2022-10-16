#pragma once

#include "headers.h"

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
		smartPointer<web::HTTPParser> parser;
		SessionsManager& session;
		const web::BaseTCPServer& serverReference;
		interfaces::IStaticFile& staticResources;
		interfaces::IDynamicFile& dynamicResources;
		sqlite::SQLiteManager& database;
		sockaddr& clientAddr;
		std::unordered_map<std::string, std::variant<std::string, int64_t>> routeParameters;

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
		static web::HTTPParser sendRequestToAnotherServer(const std::string& ip, const std::string& port, const std::string& request);

	public:
		/// @brief Construct HTTPRequest
		/// @param session Session from MultithreadedWebServer
		/// @param serverReference Reference to server
		/// @param staticResources Static resources manager
		/// @param dynamicResources Dynamic resources manager
		/// @param database Reference to database
		/// @param clientAddr Client address
		HTTPRequest(SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources, sqlite::SQLiteManager& database, sockaddr& clientAddr);

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
		const std::string& getHTTPVersion() const;

		/// <summary>
		/// All HTTP headers
		/// </summary>
		/// <returns>HTTP headers as map</returns>
		const std::unordered_map<std::string, std::string, web::HTTPParser::insensitiveStringHash, web::HTTPParser::insensitiveStringEqual>& getHeaders() const;

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
		/// <param name="response">with file</param>
		/// <exception cref="framework::exceptions::DynamicPagesSyntaxException"></exception>
		/// <exception cref="std::exception"></exception>
		void sendAssetFile(const std::string& filePath, HTTPResponse& response, const std::unordered_map<std::string_view, std::string>& variables = {});

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

		/// <summary>
		/// Get instance of SQLiteDatabaseModel subclass for database operations
		/// </summary>
		/// <typeparam name="SQLiteDatabaseModelSubclass">subclass of SQLiteDatabaseModel</typeparam>
		/// <typeparam name="...Args">arguments for constructor if needs to create new instance</typeparam>
		/// <param name="databaseName">name of database</param>
		/// <param name="tableName">name of table</param>
		/// <param name="...args">arguments for constructor if needs to create new instance</param>
		/// <returns>instance of SQLiteDatabaseModel subclass</returns>
		template<typename SQLiteDatabaseModelSubclass, typename... Args>
		smartPointer<sqlite::SQLiteDatabaseModel>& getDatabaseModelInstance(const std::string& databaseName, const std::string& tableName, Args&&... args);

		friend class ExecutorsManager;

		~HTTPRequest() = default;
	};

	template<typename SQLiteDatabaseModelSubclass, typename... Args>
	smartPointer<sqlite::SQLiteDatabaseModel>& HTTPRequest::getDatabaseModelInstance(const std::string& databaseName, const std::string& tableName, Args&&... args)
	{
		return database.get<SQLiteDatabaseModelSubclass>(databaseName, tableName, std::forward<Args>(args)...);
	}
}
