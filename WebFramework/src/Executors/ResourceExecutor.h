#pragma once

#include <array>
#include <filesystem>
#include <shared_mutex>

#include "BaseStatelessExecutor.h"
#include "DynamicPages/WebFrameworkDynamicPages.h"
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
		enum HTMLErrors
		{
			badRequest400,
			notFound404,
			internalServerError500,
			HTMLErrorsSize
		};

	private:
		std::shared_mutex cacheMutex;
		const std::filesystem::path defaultAssets;
		const std::filesystem::path assets;
		WebFrameworkDynamicPages dynamicPages;
		std::unordered_map<std::string, std::string> staticCache;
		std::unordered_map<std::string, std::string> dynamicCache;
		std::array<std::string, HTMLErrors::HTMLErrorsSize> HTMLErrorsData;
		bool isCaching;

	private:
		/// <summary>
		/// Load all htmls from WebFrameworkAssets/Errors
		/// </summary>
		void loadHTMLErrorsData();

	public:
		ResourceExecutor(const std::filesystem::path& assets, bool isCaching, const std::string& pathToTemplates);

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
		/// <exception cref="framework::exceptions::FileDoesNotExistException"></exception>
		void sendStaticFile(const std::string& filePath, HTTPResponse& response) override;

		/// <summary>
		/// Override from ISendDynamicFile interface
		/// </summary>
		/// <param name="filePath">path to file from assets folder</param>
		/// <param name="response">used for sending file</param>
		/// <exception cref="framework::exceptions::FileDoesNotExistException"></exception>
		void sendDynamicFile(const std::string& filePath, HTTPResponse& response, const std::unordered_map<std::string_view, std::string>& variables) override;

		/// <summary>
		/// Send file via GET request
		/// </summary>
		/// <param name="request">file request</param>
		/// <param name="response">response with asset file</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
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
