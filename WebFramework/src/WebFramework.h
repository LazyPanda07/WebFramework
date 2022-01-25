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
		static std::string webFrameworkVersion();

	private:
		smartPointer<BaseWebServer> server;

	public:
		/// <summary>
		/// Construct WebFramework
		/// </summary>
		/// <param name="configurationINIFile">path to .json configuration file</param>
		/// <exception cref="framework::exceptions::BaseExecutorException"></exception>
		/// <exception cref="framework::exceptions::FileDoesNotExistException"></exception>
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
		/// Get ip addresses of all currently connected clients
		/// </summary>
		/// <returns></returns>
		std::vector<std::string> getClientsIp() const;

		/// <summary>
		/// Disconnect specific client
		/// </summary>
		/// <param name="ip">client's address</param>
		void disconnectClient(const std::string& ip) const;

		/// <summary>
		/// Is server running
		/// </summary>
		/// <returns>is server running</returns>
		bool getServerState() const;

		~WebFramework() = default;
	};
}
