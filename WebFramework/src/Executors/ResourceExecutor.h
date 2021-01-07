#pragma once

#include <filesystem>
#include <shared_mutex>

#include "BaseStatelessExecutor.h"
#include "WebNetwork/Interfaces/ISendStaticFile.h"
#include "WebNetwork/Interfaces/ISendDynamicFile.h"

namespace framework
{
	/// <summary>
	/// Used for sending asset files
	/// </summary>
	class ResourceExecutor : public interfaces::ISendStaticFile, public interfaces::ISendDynamicFile, public BaseStatelessExecutor
	{
	private:
		std::shared_mutex cacheMutex;
		const std::filesystem::path defaultAssets;
		const std::filesystem::path assets;
		std::unordered_map<std::string, std::string> cache;
		bool isCaching;

	public:
		ResourceExecutor(const std::filesystem::path& assets, bool isCaching);

		/// <summary>
		/// Create assets folder
		/// </summary>
		/// <param name="settings">not used</param>
		void init(const utility::XMLSettingsParser::ExecutorSettings& settings) override;

		/// <summary>
		/// Override from ISendStaticFile interface
		/// </summary>
		/// <param name="filePath">path to file from assets folder</param>
		/// <param name="response">used for sending file</param>
		void sendStaticFile(const std::string& filePath, HTTPResponse& response) override;

		/// <summary>
		/// Override from ISendDynamicFile interface
		/// </summary>
		/// <param name="filePath">path to file from assets folder</param>
		/// <param name="response">used for sending file</param>
		void sendDynamicFile(const std::string& filePath, HTTPResponse& response) override;

		/// <summary>
		/// Send file via GET request
		/// </summary>
		/// <param name="request">file request</param>
		/// <param name="response">response with asset file</param>
		void doGet(HTTPRequest&& request, HTTPResponse& response) override;

		/// <summary>
		/// Send file via POST request
		/// </summary>
		/// <param name="request">file request</param>
		/// <param name="response">response with asset file</param>
		void doPost(HTTPRequest&& request, HTTPResponse& response) override;

		/// <summary>
		/// Send not 404.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void notFoundError(HTTPResponse& response);

		/// <summary>
		/// Send not 400.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void badRequestError(HTTPResponse& response);

		/// <summary>
		/// Send not 500.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void internalServerError(HTTPResponse& response);

		~ResourceExecutor() = default;
	};
}
