#pragma once

#ifdef WEB_FRAMEWORK_DLL
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // WEB_FRAMEWORK_DLL

#include <filesystem>

#include "WebNetwork/WebServer.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	/// <summary>
	/// <para>Main class of framework</para>
	/// <para>All initializations go through this class</para>
	/// </summary>
	class WEB_FRAMEWORK_API WebFramework
	{
	private:
		smartPointer<WebServer> server;

	public:
		/// <summary>
		/// Construct WebFramework
		/// </summary>
		/// <param name="configurationINIFile">path to .ini configuration file</param>
		/// <exception cref="framework::exceptions::BaseExecutorException"></exception>
		/// <exception cref="framework::exceptions::FileDoesNotExistException"></exception>
		/// <exception cref="std::out_of_range">Can't find some settings</exception>
		/// <exception cref="std::invalid_argument">Wrong value for settings</exception>
		WebFramework(const std::filesystem::path& configurationINIFile);

		/// <summary>
		/// Start WebServer
		/// </summary>
		void startServer();

		/// <summary>
		/// Stop WebServer incoming connections
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
