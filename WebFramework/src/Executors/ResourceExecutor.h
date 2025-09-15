#pragma once

#include "Executors/BaseStatelessExecutor.h"
#include "WebInterfaces/IStaticFile.h"
#include "WebInterfaces/IDynamicFile.h"

#include <array>

#include "FileManager.h"

#include "Framework/WebFrameworkPlatform.h"
#include "Rendering/WFDPRenderer.h"
#include "Utility/AdditionalServerSettings.h"

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
			forbidden403,
			notFound404,
			internalServerError500,
			badGateway502,
			HTMLErrorsSize
		};

	private:
		const std::filesystem::path defaultAssets;
		const std::filesystem::path assets;
		WFDPRenderer wfdpRenderer;
		std::array<std::string, HTMLErrors::HTMLErrorsSize> HTMLErrorsData;
		file_manager::FileManager& fileManager;
		std::unordered_map<std::string_view, std::unique_ptr<interfaces::IStaticFileRenderer>, interfaces::InsensitiveStringViewHash, interfaces::InsensitiveStringViewEqual> staticRenderers;

	private:
		void loadHTMLErrorsData();

		void loadStaticRenderers();

		void readFile(std::filesystem::path extension, std::string& result, std::unique_ptr<file_manager::ReadFileHandle>&& handle);

	public:
		ResourceExecutor(const json::JSONParser& configuration, const utility::AdditionalServerSettings& additionalSettings, std::shared_ptr<threading::ThreadPool> threadPool);

		/// <summary>
		/// Override from IStaticFile interface
		/// </summary>
		/// <param name="filePath">path to file from assets folder</param>
		/// <param name="response">used for sending file</param>
		/// <exception cref="file_manager::exceptions::FileDoesNotExistException"></exception>
		void sendStaticFile(std::string_view filePath, interfaces::IHTTPResponse& response, bool isBinary = true, std::string_view fileName = "") override;

		/// <summary>
		/// Override from IDynamicFile interface
		/// </summary>
		/// <param name="filePath">path to file from assets folder</param>
		/// <param name="response">used for sending file</param>
		/// <exception cref="file_manager::exceptions::FileDoesNotExistException"></exception>
		void sendDynamicFile(std::string_view filePath, interfaces::IHTTPResponse& response, std::span<const interfaces::CVariable> variables, bool isBinary = true, std::string_view fileName = "") override;

		void processWFDPFile(std::string& data, std::span<const interfaces::CVariable> variables) override;

		/// @brief Add new function in .wfdp interpreter
		/// @param functionName Name of new function
		/// @param function Function implementation
		void registerDynamicFunction(std::string_view functionName, std::function<std::string(const std::vector<std::string>&)>&& function) override;

		/// @brief Remove function from .wfdp interpreter
		/// @param functionName Name of function
		void unregisterDynamicFunction(std::string_view functionName) override;

		/// @brief Check if function is registered
		/// @param functionName Name of function
		/// @return true if function is registered, false otherwise
		bool isDynamicFunctionRegistered(std::string_view functionName) override;

		const std::filesystem::path& getPathToAssets() const override;

		const std::unordered_map<std::string_view, std::unique_ptr<interfaces::IStaticFileRenderer>, interfaces::InsensitiveStringViewHash, interfaces::InsensitiveStringViewEqual>& getStaticRenderers() const override;

		/// <summary>
		/// Send file via GET request
		/// </summary>
		/// <param name="request">file request</param>
		/// <param name="response">response with asset file</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		void doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		/// <summary>
		/// Send file via POST request
		/// </summary>
		/// <param name="request">file request</param>
		/// <param name="response">response with asset file</param>
		void doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		/// <summary>
		/// Send 404.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void notFoundError(HTTPResponseExecutors& response, const std::exception* exception = nullptr);

		/// <summary>
		/// Send 400.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void badRequestError(HTTPResponseExecutors& response, const std::exception* exception = nullptr);

		/**
		 * @brief Send 403.html from WebFrameworkAssets
		 * @param response Response with error file
		 * @param exception
		 */
		void forbiddenError(HTTPResponseExecutors& response, const std::exception* exception = nullptr);

		/// <summary>
		/// Send 500.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void internalServerError(HTTPResponseExecutors& response, const std::exception* exception = nullptr);

		/// <summary>
		/// Send 502.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void badGatewayError(HTTPResponseExecutors& response, const std::exception* exception = nullptr);

		bool getIsCaching() const;

		~ResourceExecutor() = default;
	};
}
