#pragma once

#include "headers.h"

#include "WebNetwork/WebServers/BaseWebServer.h"

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
		std::filesystem::path configurationJSONFile;
		std::filesystem::path basePath;

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
		void startServer(bool wait = false, const std::function<void()>& onStartServer = []() {});

		/// <summary>
		/// Stop BaseWebServer incoming connections
		/// </summary>
		void stopServer(bool wait = true);

		/// <summary>
		/// Kick specific client
		/// </summary>
		/// <param name="ip">client's address</param>
		void kick(const std::string& ip) const;

		/// <summary>
		/// Get ip addresses of all currently connected clients
		/// </summary>
		/// <returns></returns>
		std::vector<std::string> getClientsIp() const;

		/// <summary>
		/// Is server running
		/// </summary>
		/// <returns>is server running</returns>
		bool isServerRunning() const;

		/// @brief Getter for currentConfiguration
		/// @return Get current running configuration JSON 
		const json::JSONParser& getCurrentConfiguration() const;

		/// @brief Getter for configurationJSONFile
		/// @return Get path to configuration JSON file
		const std::filesystem::path& getConfigurationJSONFile() const;

		~WebFramework() = default;
	};
}
