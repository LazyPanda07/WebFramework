#include "ResourceExecutor.h"

#include <Log.h>

#include "Rendering/MDRenderer.h"
#include "Framework/WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/BadRequestException.h"
#include "WFDP/CXXDynamicFunction.h"

#ifdef __WITH_PYTHON_EXECUTORS__
#include "WFDP/PythonDynamicFunction.h"
#endif

#ifdef __WITH_DOT_NET_EXECUTORS__
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

	void ResourceExecutor::readFile(std::filesystem::path extension, std::string& result, std::unique_ptr<file_manager::ReadFileHandle>&& handle)
	{
		result = handle->readAllData();

		if (result.empty())
		{
			throw exceptions::BadRequestException("File is empty");
		}
	}

	ResourceExecutor::ResourceExecutor(const json::JsonParser& configuration, const utility::AdditionalServerSettings& additionalSettings, std::shared_ptr<threading::ThreadPool> threadPool) :
		defaultAssets
		(
			configuration.get<json::JsonObject>(json_settings::webFrameworkObject).contains<std::string>(json_settings::webFrameworkDefaultAssetsPath) ?
			configuration.get<json::JsonObject>(json_settings::webFrameworkObject)[json_settings::webFrameworkDefaultAssetsPath].get<std::string>() :
			webFrameworkDefaultAssests
		),
		assets(additionalSettings.assetsPath),
		wfdpRenderer(additionalSettings.templatesPath),
		fileManager(file_manager::FileManager::getInstance(threadPool))
	{
		fileManager.getCache().setCacheSize(additionalSettings.cachingSize);

		if (!std::filesystem::exists(assets))
		{
			std::filesystem::create_directories(assets);
		}

		if (!std::filesystem::exists(wfdpRenderer.getPathToTemplates()))
		{
			std::filesystem::create_directories(wfdpRenderer.getPathToTemplates());
		}

		this->loadHTMLErrorsData();
		this->loadStaticRenderers();
	}

	void ResourceExecutor::notFoundError(HTTPResponseExecutors& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::notFound404];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(std::format("{} Exception: {}", message, exception->what()).data());
		}
		else
		{
			response.setBody(message.data());
		}
#endif

		response.setResponseCode(web::ResponseCodes::notFound);
	}

	void ResourceExecutor::badRequestError(HTTPResponseExecutors& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::badRequest400];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(std::format("{} Exception: {}", message, exception->what()).data());
		}
		else
		{
			response.setBody(message.data());
		}
#endif

		response.setResponseCode(web::ResponseCodes::badRequest);
	}

	void ResourceExecutor::forbiddenError(HTTPResponseExecutors& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::forbidden403];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(std::format("{} Exception: {}", message, exception->what()).data());
		}
		else
		{
			response.setBody(message.data());
		}
#endif

		response.setResponseCode(web::ResponseCodes::forbidden);
	}

	void ResourceExecutor::internalServerError(HTTPResponseExecutors& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::internalServerError500];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(std::format("{} Exception: {}", message, exception->what()).data());
		}
		else
		{
			response.setBody(message.data());
		}
#endif

		response.setResponseCode(web::ResponseCodes::internalServerError);
	}

	void ResourceExecutor::badGatewayError(HTTPResponseExecutors& response, const std::exception* exception)
	{
		std::string_view message = HTMLErrorsData[HTMLErrors::badGateway502];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(std::format("{} Exception: {}", message, exception->what()).data());
		}
		else
		{
			response.setBody(message.data());
		}
#endif

		response.setResponseCode(web::ResponseCodes::badGateway);
	}

	bool ResourceExecutor::fileExist(const std::filesystem::path& filePath) const
	{
		return fileManager.exists(assets / filePath);
	}

	bool ResourceExecutor::getIsCaching() const
	{
		return fileManager.getCache().getCacheSize();
	}

	void ResourceExecutor::sendStaticFile(std::string_view filePath, interfaces::IHTTPResponse& response, bool isBinary, std::string_view fileName)
	{
		std::string result;
		std::filesystem::path assetFilePath(assets / filePath);

		if (!std::filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		std::filesystem::path extension = assetFilePath.extension();

		if (Log::isValid())
		{
			Log::info("Request static file: {}, is binary: {}", "LogResource", filePath, isBinary);
		}

		auto renderer = staticRenderers.find(extension.string());

		if (isBinary)
		{
			fileManager.readBinaryFile(assetFilePath, bind(&ResourceExecutor::readFile, this, std::move(extension), ref(result), std::placeholders::_1));
		}
		else
		{
			fileManager.readFile(assetFilePath, bind(&ResourceExecutor::readFile, this, std::move(extension), ref(result), std::placeholders::_1));
		}

		if (fileName.size())
		{
			response.addHeader("Content-Disposition", std::format(R"(attachment; filename="{}")", fileName).data());
		}

		if (renderer != staticRenderers.end())
		{
			result = renderer->second->render(result);
		}

		response.setBody(result.data());
	}

	void ResourceExecutor::sendDynamicFile(std::string_view filePath, interfaces::IHTTPResponse& response, std::span<const interfaces::CVariable> variables, bool isBinary, std::string_view fileName)
	{
		std::string result;
		std::filesystem::path assetFilePath(assets / filePath);

		if (!std::filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		std::filesystem::path extension = assetFilePath.extension();

		if (Log::isValid())
		{
			Log::info("Request dynamic file: {}, is binary: {}", "LogResource", filePath, isBinary);
		}

		if (isBinary)
		{
			fileManager.readBinaryFile(assetFilePath, bind(&ResourceExecutor::readFile, this, std::move(extension), ref(result), std::placeholders::_1));
		}
		else
		{
			fileManager.readFile(assetFilePath, bind(&ResourceExecutor::readFile, this, std::move(extension), ref(result), std::placeholders::_1));
		}

		wfdpRenderer.run(variables, result);

		if (fileName.size())
		{
			response.addHeader("Content-Disposition", std::format(R"(attachment; filename="{}")", fileName).data());
		}

		response.setBody(result.data());
	}

	void ResourceExecutor::processWFDPFile(std::string& data, std::span<const interfaces::CVariable> variables)
	{
		wfdpRenderer.run(variables, data);
	}

	void ResourceExecutor::registerDynamicFunction(std::string_view functionName, std::string_view apiType, const std::any& function)
	{
		static const std::unordered_map<std::string_view, std::function<std::unique_ptr<DynamicFunction>(const std::any&)>> apiDynamicFunctions =
		{
			{ json_settings::cxxExecutorKey, [](const std::any& function) { return std::make_unique<CXXDynamicFunction>(std::any_cast<std::function<std::string(const std::vector<std::string>&)>>(function)); } },
			{ json_settings::ccExecutorKey, [](const std::any& function) { return std::make_unique<CXXDynamicFunction>(std::any_cast<std::function<std::string(const std::vector<std::string>&)>>(function)); } },
#ifdef __WITH_PYTHON_EXECUTORS__
			{ json_settings::pythonExecutorKey, [](const std::any& function) { return std::make_unique<PythonDynamicFunction>(std::any_cast<void*>(function)); }},
#endif
#ifdef __WITH_DOT_NET_EXECUTORS__
			{ json_settings::csharpExecutorKey, [](const std::any& function) { return std::make_unique<CSharpDynamicFunction>(std::any_cast<char*>(function)); }},
#endif
		};

		if (Log::isValid())
		{
			Log::info("Register function: {} from: ", "LogWFDP", functionName, apiType);
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

	const std::filesystem::path& ResourceExecutor::getPathToAssets() const
	{
		return assets;
	}

	const std::unordered_map<std::string_view, std::unique_ptr<interfaces::IStaticFileRenderer>, interfaces::InsensitiveStringViewHash, interfaces::InsensitiveStringViewEqual>& ResourceExecutor::getStaticRenderers() const
	{
		return staticRenderers;
	}

	void ResourceExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		request.sendAssetFile(request.getRawParameters(), response);
	}

	void ResourceExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response) //-V524
	{
		request.sendAssetFile(request.getRawParameters(), response);
	}
}
