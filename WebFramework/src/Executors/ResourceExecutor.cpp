#include "Executors/ResourceExecutor.h"

#include <HttpBuilder.h>

#include "Rendering/MDRenderer.h"
#include "Framework/WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/BadRequestException.h"
#include "WFDP/CXXDynamicFunction.h"
#include "WFDP/CCDynamicFunction.h"
#include "Utility/ExecutorsUtility.h"
#include "Utility/Utils.h"

#ifdef __WITH_PYTHON_EXECUTORS__
#include "WFDP/PythonDynamicFunction.h"
#endif

#ifdef __WITH_DOTNET_EXECUTORS__
#include "WFDP/CSharpDynamicFunction.h"
#endif

namespace framework
{
	void ResourceExecutor::loadHTMLErrorsData()
	{
		auto readFile = [](const std::filesystem::path& errorPath) -> std::string
			{
				std::ifstream stream(errorPath);
				std::ostringstream os;
				std::string result;

				os << stream.rdbuf();

				result = os.str();

				return result;
			};
		std::filesystem::path allErrorsFolder(defaultAssets / web_framework_assets::errorsFolder);

		HTMLErrorsData[HTMLErrors::badRequest400] = readFile(allErrorsFolder / web_framework_assets::badRequest);
		HTMLErrorsData[HTMLErrors::forbidden403] = readFile(allErrorsFolder / web_framework_assets::forbidden);
		HTMLErrorsData[HTMLErrors::notFound404] = readFile(allErrorsFolder / web_framework_assets::notFound);
		HTMLErrorsData[HTMLErrors::internalServerError500] = readFile(allErrorsFolder / web_framework_assets::internalServerError);
		HTMLErrorsData[HTMLErrors::badGateway502] = readFile(allErrorsFolder / web_framework_assets::badGateway);
	}

	void ResourceExecutor::loadStaticRenderers()
	{
		std::unique_ptr<interfaces::IStaticFileRenderer> mdRenderer = std::make_unique<MDRenderer>();

		staticRenderers.try_emplace(mdRenderer->getExtension(), move(mdRenderer));
	}

	void ResourceExecutor::loadBinaryAssets(const json::JsonObject& webFrameworkObject, const utility::AdditionalServerSettings& additionalSettings, std::shared_ptr<threading::ThreadPool> threadPool)
	{
		std::vector<json::JsonObject> binaryAssets;

		if (webFrameworkObject.tryGet<std::vector<json::JsonObject>>(json_settings::binaryAssetsKey, binaryAssets))
		{
			for (const json::JsonObject& binaryAsset : binaryAssets)
			{
				if (binaryAsset.is<std::string>())
				{
					singleBinaryAssetProviders.emplace_back(additionalSettings.assetsPath, threadPool, binaryAsset.get<std::string>(), true);

					if (Log::isValid())
					{
						Log::info<logging::message::addBinaryAsset, logging::category::resource>(binaryAsset.get<std::string>(), true);
					}
				}
				else
				{
					const std::string& path = binaryAsset[json_settings::pathKey].get<std::string>();
					bool fullyLoad = true;
					
					binaryAsset.tryGet<bool>(json_settings::fullyLoadKey, fullyLoad);

					singleBinaryAssetProviders.emplace_back(additionalSettings.assetsPath, threadPool, path, fullyLoad);

					if (Log::isValid())
					{
						Log::info<logging::message::addBinaryAsset, logging::category::resource>(path, fullyLoad);
					}
				}
			}
		}
	}

	ResourceExecutor::ResourceExecutor(const json::JsonParser& configuration, const utility::AdditionalServerSettings& additionalSettings, std::shared_ptr<threading::ThreadPool> threadPool) :
		defaultAssets
		(
			configuration.get<json::JsonObject>(json_settings::webFrameworkObject).contains<std::string>(json_settings::webFrameworkDefaultAssetsPath) ?
			configuration.get<json::JsonObject>(json_settings::webFrameworkObject)[json_settings::webFrameworkDefaultAssetsPath].get<std::string>() :
			webFrameworkDefaultAssests
		),
		defaultAssetProvider(additionalSettings.assetsPath, threadPool),
		wfdpRenderer(additionalSettings.templatesPath, additionalSettings.dynamicFunctionValidation)
	{
		file_manager::FileManager::getInstance().getCache().setCacheSize(additionalSettings.cachingSize);

		if (!std::filesystem::exists(additionalSettings.assetsPath))
		{
			std::filesystem::create_directories(additionalSettings.assetsPath);
		}

		if (!std::filesystem::exists(wfdpRenderer.getPathToTemplates()))
		{
			std::filesystem::create_directories(wfdpRenderer.getPathToTemplates());
		}

		this->loadHTMLErrorsData();
		this->loadStaticRenderers();
		this->loadBinaryAssets(configuration.get<json::JsonObject>(json_settings::webFrameworkObject), additionalSettings, threadPool);
	}

	void ResourceExecutor::notFoundError(interfaces::IHttpResponse& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::notFound404];

#ifdef NDEBUG
		response.setBody(message.data(), message.size());
#else
		if (exception)
		{
			std::string exceptionMessage = std::format("{} Exception: {}", message, exception->what());

			response.setBody(exceptionMessage.data(), exceptionMessage.size());
		}
		else
		{
			response.setBody(message.data(), message.size());
		}
#endif

		response.setResponseCode(static_cast<int64_t>(web::ResponseCodes::notFound));
	}

	void ResourceExecutor::badRequestError(interfaces::IHttpResponse& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::badRequest400];

#ifdef NDEBUG
		response.setBody(message.data(), message.size());
#else
		if (exception)
		{
			std::string exceptionMessage = std::format("{} Exception: {}", message, exception->what());

			response.setBody(exceptionMessage.data(), exceptionMessage.size());
		}
		else
		{
			response.setBody(message.data(), message.size());
		}
#endif

		response.setResponseCode(static_cast<int64_t>(web::ResponseCodes::badRequest));
	}

	void ResourceExecutor::forbiddenError(interfaces::IHttpResponse& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::forbidden403];

#ifdef NDEBUG
		response.setBody(message.data(), message.size());
#else
		if (exception)
		{
			std::string exceptionMessage = std::format("{} Exception: {}", message, exception->what());

			response.setBody(exceptionMessage.data(), exceptionMessage.size());
		}
		else
		{
			response.setBody(message.data(), message.size());
		}
#endif

		response.setResponseCode(static_cast<int64_t>(web::ResponseCodes::forbidden));
	}

	void ResourceExecutor::internalServerError(interfaces::IHttpResponse& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::internalServerError500];

#ifdef NDEBUG
		response.setBody(message.data(), message.size());
#else
		if (exception)
		{
			std::string exceptionMessage = std::format("{} Exception: {}", message, exception->what());

			response.setBody(exceptionMessage.data(), exceptionMessage.size());
		}
		else
		{
			response.setBody(message.data(), message.size());
		}
#endif

		response.setResponseCode(static_cast<int64_t>(web::ResponseCodes::internalServerError));
	}

	void ResourceExecutor::badGatewayError(interfaces::IHttpResponse& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::badGateway502];

#ifdef NDEBUG
		response.setBody(message.data(), message.size());
#else
		if (exception)
		{
			std::string exceptionMessage = std::format("{} Exception: {}", message, exception->what());

			response.setBody(exceptionMessage.data(), exceptionMessage.size());
		}
		else
		{
			response.setBody(message.data(), message.size());
		}
#endif

		response.setResponseCode(static_cast<int64_t>(web::ResponseCodes::badGateway));
	}

	bool ResourceExecutor::fileExist(const std::filesystem::path& filePath) const
	{
		if (utility::escapeFromAssets(filePath.string()))
		{
			return false;
		}

		if (std::ranges::any_of(singleBinaryAssetProviders, [&filePath](const asset::SingleBinaryAssetProvider& provider) { return provider.exists(filePath); }))
		{
			return true;
		}

		return defaultAssetProvider.exists(filePath);
	}

	bool ResourceExecutor::getIsCaching() const
	{
		return file_manager::FileManager::getInstance().getCache().getCacheSize();
	}

	void ResourceExecutor::sendStaticFile(std::string_view filePath, interfaces::IHttpResponse& response, std::string_view fileName)
	{
		if (utility::escapeFromAssets(filePath))
		{
			this->forbiddenError(response);

			return;
		}

		std::string result = this->getFile(filePath);

		if (fileName.size())
		{
			response.addHeader("Content-Disposition", std::format(R"(attachment; filename="{}")", fileName).data());
		}

		if (auto renderer = staticRenderers.find(std::filesystem::path(filePath).extension().string()); renderer != staticRenderers.end())
		{
			result = renderer->second->render(result);
		}

		response.setBody(result.data(), result.size());

		if (Log::isValid())
		{
			Log::info<logging::message::requestStaticFile, logging::category::resource>(filePath);
		}
	}

	void ResourceExecutor::sendDynamicFile(std::string_view filePath, interfaces::IHttpResponse& response, const void* arguments, std::string_view fileName)
	{
		if (utility::escapeFromAssets(filePath))
		{
			this->forbiddenError(response);

			return;
		}

		std::string result = this->getFile(filePath);

		wfdpRenderer.run(arguments, result);

		if (fileName.size())
		{
			response.addHeader("Content-Disposition", std::format(R"(attachment; filename="{}")", fileName).data());
		}

		response.setBody(result.data(), result.size());

		if (Log::isValid())
		{
			Log::info<logging::message::requestDynamicFile, logging::category::resource>(filePath);
		}
	}

	void ResourceExecutor::processDynamicFile(std::string& data, const void* arguments)
	{
		wfdpRenderer.run(arguments, data);
	}

	void ResourceExecutor::registerDynamicFunction(std::string_view functionName, std::string_view apiType, const std::any& function)
	{
		static const std::unordered_map<std::string_view, std::function<std::unique_ptr<DynamicFunction>(const std::any&)>> apiDynamicFunctions =
		{
			{ json_settings::cxxExecutorKey,[](const std::any& function) { return std::make_unique<CXXDynamicFunction>(std::any_cast<void*>(function)); } },
			{ json_settings::ccExecutorKey,[](const std::any& function) { return std::make_unique<CCDynamicFunction>(std::any_cast<void*>(function)); } },
#ifdef __WITH_PYTHON_EXECUTORS__
			{ json_settings::pythonExecutorKey, [](const std::any& function) { return std::make_unique<PythonDynamicFunction>(std::any_cast<void*>(function)); }},
#endif
#ifdef __WITH_DOTNET_EXECUTORS__
			{ json_settings::csharpExecutorKey, [](const std::any& function) { return std::make_unique<CSharpDynamicFunction>(std::any_cast<char*>(function)); }},
#endif
		};

		if (Log::isValid())
		{
			Log::info<logging::message::registerFunction, logging::category::dynamicFunction>(functionName, apiType);
		}

		wfdpRenderer.registerDynamicFunction(functionName, apiDynamicFunctions.at(apiType)(function));
	}

	void ResourceExecutor::unregisterDynamicFunction(std::string_view functionName)
	{
		wfdpRenderer.unregisterDynamicFunction(functionName);
	}

	bool ResourceExecutor::isDynamicFunctionRegistered(std::string_view functionName)
	{
		return wfdpRenderer.isDynamicFunctionRegistered(functionName);
	}

	std::string ResourceExecutor::getFile(std::string_view filePath)
	{
		if (utility::escapeFromAssets(filePath))
		{
			throw std::runtime_error(std::format("Escape from assets: {}", filePath));

			// TODO: forbidden error

			return "";
		}

		std::string result;

		for (asset::SingleBinaryAssetProvider& provider : singleBinaryAssetProviders)
		{
			if (provider.exists(filePath))
			{
				provider.getAsset(filePath, result);

				break;
			}
		}

		if (result.empty())
		{
			defaultAssetProvider.getAsset(filePath, result);
		}

		return result;
	}

	std::unique_ptr<std::istream> ResourceExecutor::getFileStream(std::string_view filePath)
	{
		if (utility::escapeFromAssets(filePath))
		{
			throw std::runtime_error(std::format("Escape from assets: {}", filePath));

			// TODO: forbidden error

			return nullptr;
		}

		std::unique_ptr<std::istream> result;

		for (asset::SingleBinaryAssetProvider& provider : singleBinaryAssetProviders)
		{
			if (provider.exists(filePath))
			{
				result = provider.getAssetStream(filePath);

				break;
			}
		}

		if (!result)
		{
			result = defaultAssetProvider.getAssetStream(filePath);
		}

		return result;
	}

	const std::unordered_map<std::string_view, std::unique_ptr<interfaces::IStaticFileRenderer>, interfaces::InsensitiveStringViewHash, interfaces::InsensitiveStringViewEqual>& ResourceExecutor::getStaticRenderers() const
	{
		return staticRenderers;
	}

	void ResourceExecutor::doGet(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		request.sendAssetFile(request.getRawParameters(), &response);
	}

	void ResourceExecutor::doPost(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) //-V524
	{
		request.sendAssetFile(request.getRawParameters(), &response);
	}
}
