#pragma once

#include "HTTPParser.h"

#include "SQLite3/SQLiteManager.h"
#include "WebNetwork/Interfaces/IStaticFile.h"
#include "WebNetwork/Interfaces/IDynamicFile.h"
#include "WebNetwork/Interfaces/IHTTPRequest.h"
#include "Utility/ChunkGenerator.h"
#include "WebFrameworkCoreConstants.h"
#include "IOSocketStream.h"
#include "HTTPBuilder.h"
#include "HTTPResponseImplementation.h"

namespace web
{
	class BaseTCPServer;
}

namespace framework::utility
{
	class BaseLargeBodyHandler;
}

namespace framework
{
	class SessionsManager;

	/// <summary>
	/// Parsing HTTP request
	/// <para>Accessing to sessions</para>
	/// <para>Overriding input stream operator for simplify HTTP request initializing</para>
	/// </summary>
	class WEB_FRAMEWORK_CORE_API HTTPRequestImplementation : public interfaces::IHTTPRequest
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
		interfaces::CLargeData largeData;

	private:
		static bool isWebFrameworkDynamicPages(std::string_view filePath);

		static void logWebFrameworkModelsError(std::string_view typeName);

	private:
		void setParser(const web::HTTPParser& parser);

	public:
		static web::HTTPParser sendRequestToAnotherServer(std::string_view ip, std::string_view port, std::string_view request, DWORD timeout = 30'000, bool useHTTPS = false);

	public:
		HTTPRequestImplementation(SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources, sqlite::SQLiteManager& database, sockaddr clientAddr, streams::IOSocketStream& stream);

		HTTPRequestImplementation(HTTPRequestImplementation&&) noexcept = default;

		HTTPRequestImplementation(const HTTPRequestImplementation&) = default;

		HTTPRequestImplementation& operator =(HTTPRequestImplementation&&) noexcept = default;

		HTTPRequestImplementation& operator =(const HTTPRequestImplementation&) = default;

		void updateLargeData(const char* dataPart, size_t dataPartSize, size_t bodySize) override;

		/// <summary>
		/// Parameters string from HTTP
		/// </summary>
		/// <returns>HTTP parameters</returns>
		const char* getRawParameters() const override;

		/// <summary>
		/// HTTP request method
		/// </summary>
		/// <returns>HTTP method</returns>
		const char* getMethod() const override;

		void getKeyValueParameters(void(*addKeyValue)(const char* key, const char* value, void* additionalData), void* additionalData) const override;

		/// <summary>
		/// GET parameters
		/// </summary>
		/// <returns>GET parameters as map</returns>
		const char* getKeyValueParameter(const char* key) const override;

		/// <summary>
		/// HTTP version
		/// </summary>
		/// <returns>HTTP version</returns>
		double getHTTPVersion() const override;

		void getHeaders(void(*addHeader)(const char* key, const char* value, void* additionalData), void* additionalData) const override;

		/// <summary>
		/// All HTTP headers
		/// </summary>
		/// <returns>HTTP headers as map</returns>
		const char* getHeaderValue(const char* headerName, bool throwException = true) const override;

		/// <summary>
		/// HTTP request body
		/// </summary>
		/// <returns>HTTP request body</returns>
		const char* getBody() const override;

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name">attribute name</param>
		/// <param name="value">attribute value</param>
		void setAttribute(const char* name, const char* value) override;

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name">attribute name</param>
		/// <returns>attribute value</returns>
		/// <exception cref="std::out_of_range"></exception>
		const char* getAttribute(const char* name) override;

		void deleteAttribute(const char* attribute) override;

		/// <summary>
		/// Session wrapper
		/// </summary>
		void deleteSession();

		/// <summary>
		/// Session wrapper
		/// </summary>
		/// <param name="name"></param>
		void removeAttribute(const char* name) override;

		/// <summary>
		/// Client's cookies
		/// </summary>
		/// <returns>HTTP cookies as map</returns>
		void getCookies(void(*addCookie)(const char* key, const char* value, void* additionalData), void* additionalData) const override;

		/**
		 * @brief Get data from multipart/form-data
		 * @return
		 */
		void getMultiparts(void(*addMultipart)(const interfaces::CMultipart* part, void* additionalData), void* additionalData) const override;

		const interfaces::CLargeData* getLargeData() const override;

		/// <summary>
		/// ResourceExecutor wrapper
		/// </summary>
		/// <param name="filePath">path to asset file from assets folder</param>
		/// <param name="fileName">Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required</param>
		/// <exception cref="framework::exceptions::DynamicPagesSyntaxException"></exception>
		/// <exception cref="std::exception"></exception>
		void sendAssetFile(const char* filePath, interfaces::IHTTPResponse* response, size_t variablesSize = 0, const interfaces::CVariable* variables = nullptr, bool isBinary = true, const char* fileName = "") override;

		/**
		* Send non dynamic file
		* @param filePath Path to asset file from assets folder
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required
		* @exception std::exception
		*/
		void sendStaticFile(const char* filePath, interfaces::IHTTPResponse* response, bool isBinary = true, const char* fileName = "") override;

		/**
		* Send dynamic file(.wfdp)
		* @param filePath Path to asset file from assets folder
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required
		* @exception framework::exceptions::DynamicPagesSyntaxException
		* @exception std::exception
		*/
		void sendDynamicFile(const char* filePath, interfaces::IHTTPResponse* response, size_t variablesSize, const interfaces::CVariable* variables, bool isBinary = false, const char* fileName = "") override;

		/**
		* Send large files
		* @param filePath Path to asset file from assets folder
		* @param fileName Name of file in Content-Disposition HTTP header, ASCII name required
		* @param chunkSize Desired size of read data before sending
		*/
		void streamFile(const char* filePath, interfaces::IHTTPResponse* response, const char* fileName, size_t chunkSize = defaultChunkSize) override;

		/// @brief Add new function in .wfdp interpreter
		/// @param functionName Name of new function
		/// @param function Function implementation
		void registerDynamicFunction(const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(const char* result)) override;

		/// @brief Remove function from .wfdp interpreter
		/// @param functionName Name of function
		void unregisterDynamicFunction(const char* functionName) override;

		/// @brief Check if function is registered
		/// @param functionName Name of function
		/// @return true if function is registered, false otherwise
		bool isDynamicFunctionRegistered(const char* functionName) override;
		
		void sendFileChunks(interfaces::IHTTPResponse* response, const char* fileName, void* chunkGenerator, const char* (*getChunk)(void* chunkGenerator)) override;

		/**
		 * @brief Get chunks
		 * @return
		 */
		void getChunks(void(*addChunk)(const char* chunk, size_t chunkSize, void* additionalData), void* additionalData) const override;

		/// <summary>
		/// Getter for JSONParser
		/// </summary>
		/// <returns>JSONParser</returns>
		const char* getJSON() const override;

		const char* getRawRequest() const override;

		/// <summary>
		/// Get client's address
		/// </summary>
		/// <returns>client's address</returns>
		const char* getClientIpV4() const override;

		void deleteClientIpV4(const char* ip) const override;

		/// <summary>
		/// Get server's address
		/// </summary>
		/// <returns>server's address</returns>
		const char* getServerIpV4() const override;

		void deleteServerIpV4(const char* ip) const override;

		/// <summary>
		/// Get client's port
		/// </summary>
		/// <returns>client's port</returns>
		uint16_t getClientPort() const override;

		/// <summary>
		/// Get server's port
		/// </summary>
		/// <returns>server's port</returns>
		uint16_t getServerPort() const override;

		const char* getRouteParameterString(const char* routeParameterName) const override;

		int64_t getRouteParameterInteger(const char* routeParameterName) const override;

		double getRouteParameterDouble(const char* routeParameterName) const override;

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

		/// <summary>
		/// Reading HTTP request from network
		/// </summary>
		/// <param name="stream">special class for taking data from network</param>
		/// <param name="request">class instance</param>
		/// <returns>self for builder pattern</returns>
		/// <exception cref="web::WebException"></exception>
		friend streams::IOSocketStream& operator >> (streams::IOSocketStream& stream, HTTPRequestImplementation& request);

		/// <summary>
		/// Logging operator
		/// </summary>
		/// <param name="stream">any output source</param>
		/// <param name="request">class instance</param>
		/// <returns>self for builder pattern</returns>
		friend std::ostream& operator << (std::ostream& stream, const HTTPRequestImplementation& request);

		~HTTPRequestImplementation() = default;

		friend class ExecutorsManager;
		friend class utility::BaseLargeBodyHandler;
	};

	template<std::derived_from<sqlite::SQLiteDatabaseModel> T, typename... Args>
	std::shared_ptr<T> HTTPRequestImplementation::createModel(Args&&... args)
	{
		return database.add<T>(std::forward<Args>(args)...);
	}

	template<std::derived_from<sqlite::SQLiteDatabaseModel> T>
	std::shared_ptr<T> HTTPRequestImplementation::getModel() const
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
			HTTPRequestImplementation::logWebFrameworkModelsError(typeid(T).name());
		}

		return result;
	}
}
