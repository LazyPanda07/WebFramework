#pragma once

#include "headers.h"

#include "BaseStatelessExecutor.h"
#include "DynamicPages/WebFrameworkDynamicPages.h"
#include "WebNetwork/Interfaces/IStaticFile.h"
#include "WebNetwork/Interfaces/IDynamicFile.h"

namespace framework
{
	/// <summary>
	/// Used for sending asset files
	/// </summary>
	class ResourceExecutor : public interfaces::IStaticFile, public interfaces::IDynamicFile, public BaseStatelessExecutor
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
		const std::filesystem::path defaultAssets;
		const std::filesystem::path assets;
		WebFrameworkDynamicPages dynamicPages;
		std::array<std::string, HTMLErrors::HTMLErrorsSize> HTMLErrorsData;
		file_manager::FileManager& fileManager;

	private:
		/// <summary>
		/// Load all .htmls from WebFrameworkAssets/Errors
		/// </summary>
		void loadHTMLErrorsData();

		void readFile(std::string& result, std::unique_ptr<file_manager::ReadFileHandle>&& handle);

	public:
		ResourceExecutor(const json::JSONParser& configuration, const std::filesystem::path& assets, uint64_t cachingSize, const std::string& pathToTemplates);

		/// <summary>
		/// Create assets folder
		/// </summary>
		/// <param name="settings">not used</param>
		void init(const utility::JSONSettingsParser::ExecutorSettings& settings) override;

		/// <summary>
		/// Override from IStaticFile interface
		/// </summary>
		/// <param name="filePath">path to file from assets folder</param>
		/// <param name="response">used for sending file</param>
		/// <exception cref="file_manager::exceptions::FileDoesNotExistException"></exception>
		void sendStaticFile(const std::string& filePath, HTTPResponse& response, bool isBinary = true, const std::string& fileName = "") override;

		/// <summary>
		/// Override from IDynamicFile interface
		/// </summary>
		/// <param name="filePath">path to file from assets folder</param>
		/// <param name="response">used for sending file</param>
		/// <exception cref="file_manager::exceptions::FileDoesNotExistException"></exception>
		void sendDynamicFile(const std::string& filePath, HTTPResponse& response, const std::unordered_map<std::string_view, std::string>& variables = {}, bool isBinary = true, const std::string& fileName = "") override;

		/// @brief Add new function in .wfdp interpreter
		/// @param functionName Name of new function
		/// @param function Function implementation
		void registerDynamicFunction(const std::string& functionName, std::function<std::string(const std::vector<std::string>&)>&& function) override;

		/// @brief Remove function from .wfdp interpreter
		/// @param functionName Name of function
		void unregisterDynamicFunction(const std::string& functionName) override;

		/// @brief Check if function is registered
		/// @param functionName Name of function
		/// @return true if function is registered, false otherwise
		bool isDynamicFunctionRegistered(const std::string& functionName) override;

		/// <summary>
		/// Send file via GET request
		/// </summary>
		/// <param name="request">file request</param>
		/// <param name="response">response with asset file</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		void doGet(HTTPRequest& request, HTTPResponse& response) override;

		/// <summary>
		/// Send file via POST request
		/// </summary>
		/// <param name="request">file request</param>
		/// <param name="response">response with asset file</param>
		void doPost(HTTPRequest& request, HTTPResponse& response) override;

		const std::filesystem::path& getPathToAssets() const final override;

		/// <summary>
		/// Send 404.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void notFoundError(HTTPResponse& response, const std::exception* exception);

		/// <summary>
		/// Send 400.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void badRequestError(HTTPResponse& response, const std::exception* exception);

		/// <summary>
		/// Send 500.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void internalServerError(HTTPResponse& response, const std::exception* exception);

		bool getIsCaching() const;

		~ResourceExecutor() = default;
	};
}
