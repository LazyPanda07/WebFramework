#pragma once

#include "BaseTCPServer.h"
#include "WebFrameworkConstants.h"
#include "Managers/ExecutorsManager.h"
#include "Managers/SessionsManager.h"
#include "Utility/XMLSettingsParser.h"

namespace framework
{
	/// <summary>
	/// <para>Standard TCP web server</para>
	/// <para>Initialize only through WebFramework</para>
	/// </summary>
	class WebServer : public web::BaseTCPServer
	{
	private:
		ExecutorsManager executorsManager;
		SessionsManager sessionsManager;

	private:
		/// <summary>
		/// Called for every client
		/// </summary>
		/// <param name="clientSocket"></param>
		/// <param name="addr"></param>
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		/// <summary>
		/// Construct WebServer
		/// </summary>
		/// <param name="parser">settings from .xml file</param>
		/// <param name="assets">path to assets folder</param>
		/// <param name="pathToTemplates">path to templates folder</param>
		/// <param name="isCaching">is caching resource files</param>
		/// <param name="port">default is 80 or 443</param>
		/// <param name="timeout">wait for client connection in milliseconds</param>
		/// <param name="pathToSources">paths to files with executors</param>
		WebServer(const utility::XMLSettingsParser& parser, const std::filesystem::path& assets, const std::string& pathToTemplates, bool isCaching, const std::string_view& port = HTTPPort, DWORD timeout = 0, const std::vector<std::string>& pathToSources = {});

		~WebServer() = default;
	};
}
