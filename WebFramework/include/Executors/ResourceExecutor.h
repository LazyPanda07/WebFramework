#pragma once

#include "Executors/StatelessExecutor.h"
#include "WebInterfaces/IStaticFile.h"
#include "WebInterfaces/IDynamicFile.h"

#include <array>

#include <FileManager.h>

#include "Framework/WebFrameworkPlatform.h"
#include "Rendering/WFDPRenderer.h"
#include "Utility/AdditionalServerSettings.h"
#include "Assets/DefaultAssetProvider.h"
#include "Assets/SingleBinaryAssetProvider.h"

namespace framework
{
	/// <summary>
	/// Used for sending asset files
	/// </summary>
	class ResourceExecutor : public interfaces::IStaticFile, public interfaces::IDynamicFile, public StatelessExecutor
	{
	private:
		asset::DefaultAssetProvider defaultAssetProvider;
		std::vector<asset::SingleBinaryAssetProvider> singleBinaryAssetProviders;
		WFDPRenderer wfdpRenderer;
		std::unordered_map<std::string_view, std::unique_ptr<interfaces::IStaticFileRenderer>, interfaces::InsensitiveStringViewHash, interfaces::InsensitiveStringViewEqual> staticRenderers;

	private:
		void loadStaticRenderers();

		void loadBinaryAssets(const json::JsonObject& webFrameworkObject, const utility::AdditionalServerSettings& additionalSettings, std::shared_ptr<threading::ThreadPool> threadPool);

	public:
		ResourceExecutor(const json::JsonParser& configuration, const utility::AdditionalServerSettings& additionalSettings, std::shared_ptr<threading::ThreadPool> threadPool);

		/// <summary>
		/// Send 404.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void notFoundError(interfaces::IHttpResponse& response, const std::exception* exception = nullptr);

		/// <summary>
		/// Send 400.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void badRequestError(interfaces::IHttpResponse& response, const std::exception* exception = nullptr);

		/**
		 * @brief Send 403.html from WebFrameworkAssets
		 * @param response Response with error file
		 * @param exception
		 */
		void forbiddenError(interfaces::IHttpResponse& response, const std::exception* exception = nullptr);

		/// <summary>
		/// Send 500.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void internalServerError(interfaces::IHttpResponse& response, const std::exception* exception = nullptr);

		/// <summary>
		/// Send 502.html from WebFrameworkAssets
		/// </summary>
		/// <param name="response">response with error file</param>
		void badGatewayError(interfaces::IHttpResponse& response, const std::exception* exception = nullptr);

		bool fileExist(const std::filesystem::path& filePath) const;

		bool getIsCaching() const;

		void sendStaticFile(std::string_view filePath, interfaces::IHttpResponse& response, std::string_view fileName = "") override;

		void sendDynamicFile(std::string_view filePath, interfaces::IHttpResponse& response, const void* arguments, std::string_view fileName = "") override;

		void processDynamicFile(std::string& data, const void* arguments) override;

		/// @brief Add new function in .wfdp interpreter
		/// @param functionName Name of new function
		/// @param function Function implementation
		void registerDynamicFunction(std::string_view functionName, std::string_view apiType, const std::any& function) override;

		/// @brief Remove function from .wfdp interpreter
		/// @param functionName Name of function
		void unregisterDynamicFunction(std::string_view functionName) override;

		/// @brief Check if function is registered
		/// @param functionName Name of function
		/// @return true if function is registered, false otherwise
		bool isDynamicFunctionRegistered(std::string_view functionName) override;

		std::string getFile(std::string_view filePath) override;

		std::unique_ptr<std::istream> getFileStream(std::string_view filePath) override;

		const std::unordered_map<std::string_view, std::unique_ptr<interfaces::IStaticFileRenderer>, interfaces::InsensitiveStringViewHash, interfaces::InsensitiveStringViewEqual>& getStaticRenderers() const override;

		/// <summary>
		/// Send file via GET request
		/// </summary>
		/// <param name="request">file request</param>
		/// <param name="response">response with asset file</param>
		/// <exception cref="framework::exceptions::NotImplementedException"></exception>
		void doGet(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		/// <summary>
		/// Send file via POST request
		/// </summary>
		/// <param name="request">file request</param>
		/// <param name="response">response with asset file</param>
		void doPost(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		~ResourceExecutor() = default;
	};
}
