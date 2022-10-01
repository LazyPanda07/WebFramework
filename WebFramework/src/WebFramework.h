#pragma once

#include "headers.h"

#include "WebNetwork/WebServers/BaseWebServer.h"

#pragma comment(lib, "BaseTCPServer.lib")
#pragma comment(lib, "FileManager.lib")
#pragma comment(lib, "HTTP.lib")
#pragma comment(lib, "INIParser.lib")
#pragma comment(lib, "JSON.lib")
#pragma comment(lib, "Localization.lib")
#pragma comment(lib, "Log.lib")
#pragma comment(lib, "Networks.lib")
#pragma comment(lib, "SHA256.lib")
#pragma comment(lib, "SocketStreams.lib")
#pragma comment(lib, "ThreadPool.lib")

namespace framework
{
	/// <summary>
	/// Main class of framework
	/// <para>All initializations go through this class</para>
	/// </summary>
	class WEB_FRAMEWORK_API WebFramework
	{
	public:
		/// @brief Get current WebFramework version
		/// @return Current WebFramework version
		static std::string getWebFrameworkVersion();

	private:
		smartPointer<BaseWebServer> server;
		json::JSONParser currentConfiguration;

	public:
		/// <summary>
		/// Construct WebFramework
		/// </summary>
		/// <param name="configurationINIFile">path to .json configuration file</param>
		/// <exception cref="framework::exceptions::BaseExecutorException"></exception>
		/// <exception cref="file_manager::exceptions::FileDoesNotExistException"></exception>
		/// <exception cref="framework::exceptions::NotImplemented">Thread pool model does not support HTTPS</exception>
		/// <exception cref="json::exceptions::CantFindValueException">can't find JSON setting value</exception>
		WebFramework(const std::filesystem::path& configurationJSONFile);

		/// <summary>
		/// Start BaseWebServer
		/// </summary>
		void startServer();

		/// <summary>
		/// Stop BaseWebServer incoming connections
		/// </summary>
		void stopServer();

		/// <summary>
		/// Disconnect specific client
		/// </summary>
		/// <param name="ip">client's address</param>
		void disconnectClient(const std::string& ip) const;

		/// <summary>
		/// Get ip addresses of all currently connected clients
		/// </summary>
		/// <returns></returns>
		std::vector<std::string> getClientsIp() const;

		/// <summary>
		/// Is server running
		/// </summary>
		/// <returns>is server running</returns>
		bool getServerState() const;

		/// @brief Getter for currentConfiguration
		/// @return Get current running configuration JSON 
		const json::JSONParser& getCurrentConfiguration() const;

		~WebFramework() = default;
	};
}
