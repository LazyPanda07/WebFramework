#pragma once

#ifdef WEB_FRAMEWORK_DLL
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // WEB_FRAMEWORK_DLL

#include <filesystem>

#include "WebNetwork/MultithreadedWebServer.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	/// <summary>
	/// Main class of framework
	/// <para>All initializations go through this class</para>
	/// </summary>
	class WEB_FRAMEWORK_API WebFramework
	{
	private:
		smartPointer<web::BaseTCPServer> server;

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
		/// Start MultithreadedWebServer
		/// </summary>
		void startServer();

		/// <summary>
		/// Stop MultithreadedWebServer incoming connections
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
