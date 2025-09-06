#include "ResourceExecutor.h"

#include <cmark-gfm.h>

#include "Framework/WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/BadRequestException.h"
#include "Log.h"

using namespace std;

namespace framework
{
	void ResourceExecutor::loadHTMLErrorsData()
	{
		filesystem::path allErrorsFolder(defaultAssets / web_framework_assets::errorsFolder);
		ifstream html(allErrorsFolder / web_framework_assets::badRequest);
		auto readFile = [](ifstream& html) -> string
			{
				string result;
				string tem;

				while (getline(html, tem))
				{
					result += tem + '\n';
				}

				html.close();

				return result;
			};

		HTMLErrorsData[HTMLErrors::badRequest400] = readFile(html);

		html.open(allErrorsFolder / web_framework_assets::notFound);

		HTMLErrorsData[HTMLErrors::forbidden403] = readFile(html);

		html.open(allErrorsFolder / web_framework_assets::forbidden);

		HTMLErrorsData[HTMLErrors::notFound404] = readFile(html);

		html.open(allErrorsFolder / web_framework_assets::internalServerError);

		HTMLErrorsData[HTMLErrors::internalServerError500] = readFile(html);

		html.open(allErrorsFolder / web_framework_assets::badGateway);

		HTMLErrorsData[HTMLErrors::badGateway502] = readFile(html);
	}

	void ResourceExecutor::readFile(filesystem::path extension, string& result, unique_ptr<file_manager::ReadFileHandle>&& handle)
	{
		result = handle->readAllData();

		if (extension == markdownExtension)
		{
			char* temp = cmark_markdown_to_html(result.data(), result.size(), CMARK_OPT_UNSAFE);

			result = temp;

			free(temp);
		}

		if (result.empty())
		{
			throw exceptions::BadRequestException("File is empty");
		}
	}

	ResourceExecutor::ResourceExecutor(const json::JSONParser& configuration, const utility::AdditionalServerSettings& additionalSettings, shared_ptr<threading::ThreadPool> threadPool) :
		defaultAssets
		(
			configuration.getObject(json_settings::webFrameworkObject).contains(json_settings::webFrameworkDefaultAssetsPath, json::utility::variantTypeEnum::jString) ?
			configuration.getObject(json_settings::webFrameworkObject).getString(json_settings::webFrameworkDefaultAssetsPath) :
			webFrameworkDefaultAssests
		),
		assets(additionalSettings.assetsPath),
		dynamicPages(additionalSettings.templatesPath),
		fileManager(file_manager::FileManager::getInstance(threadPool))
	{
		fileManager.getCache().setCacheSize(additionalSettings.cachingSize);

		if (!filesystem::exists(assets))
		{
			filesystem::create_directories(assets);
		}

		if (!filesystem::exists(dynamicPages.getPathToTemplates()))
		{
			filesystem::create_directories(dynamicPages.getPathToTemplates());
		}

		this->loadHTMLErrorsData();
	}

	void ResourceExecutor::sendStaticFile(string_view filePath, interfaces::IHTTPResponse& response, bool isBinary, string_view fileName)
	{
		string result;
		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		filesystem::path extension = assetFilePath.extension();

		if (Log::isValid())
		{
			Log::info("Request static file: {}, is binary: {}", "LogResource", filePath, isBinary);
		}

		if (isBinary)
		{
			fileManager.readBinaryFile(assetFilePath, bind(&ResourceExecutor::readFile, this, move(extension), ref(result), placeholders::_1));
		}
		else
		{
			fileManager.readFile(assetFilePath, bind(&ResourceExecutor::readFile, this, move(extension), ref(result), placeholders::_1));
		}

		if (fileName.size())
		{
			response.addHeader("Content-Disposition", format(R"(attachment; filename="{}")", fileName).data());
		}

		response.setBody(result.data());
	}

	void ResourceExecutor::sendDynamicFile(string_view filePath, interfaces::IHTTPResponse& response, span<const interfaces::CVariable> variables, bool isBinary, string_view fileName)
	{
		string result;
		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		filesystem::path extension = assetFilePath.extension();

		if (Log::isValid())
		{
			Log::info("Request dynamic file: {}, is binary: {}", "LogResource", filePath, isBinary);
		}

		if (isBinary)
		{
			fileManager.readBinaryFile(assetFilePath, bind(&ResourceExecutor::readFile, this, move(extension), ref(result), placeholders::_1));
		}
		else
		{
			fileManager.readFile(assetFilePath, bind(&ResourceExecutor::readFile, this, move(extension), ref(result), placeholders::_1));
		}

		dynamicPages.run(variables, result);

		if (fileName.size())
		{
			response.addHeader("Content-Disposition", format(R"(attachment; filename="{}")", fileName).data());
		}

		response.setBody(result.data());
	}

	void ResourceExecutor::registerDynamicFunction(string_view functionName, function<string(const vector<string>&)>&& function)
	{
		dynamicPages.registerDynamicFunction(functionName, move(function));
	}

	void ResourceExecutor::unregisterDynamicFunction(string_view functionName)
	{
		dynamicPages.unregisterDynamicFunction(functionName);
	}

	bool ResourceExecutor::isDynamicFunctionRegistered(string_view functionName)
	{
		return dynamicPages.isDynamicFunctionRegistered(functionName);
	}

	const filesystem::path& ResourceExecutor::getPathToAssets() const
	{
		return assets;
	}

	void ResourceExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		request.sendAssetFile(request.getRawParameters(), response);
	}

	void ResourceExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response) //-V524
	{
		request.sendAssetFile(request.getRawParameters(), response);
	}

	void ResourceExecutor::notFoundError(HTTPResponseExecutors& response, const exception* exception)
	{
		string_view message = HTMLErrorsData[HTMLErrors::notFound404];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(format("{} Exception: {}", message, exception->what()).data());
		}
		else
		{
			response.setBody(message.data());
		}
#endif

		response.setResponseCode(web::ResponseCodes::notFound);
	}

	void ResourceExecutor::badRequestError(HTTPResponseExecutors& response, const exception* exception)
	{
		string_view message = HTMLErrorsData[HTMLErrors::badRequest400];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(format("{} Exception: {}", message, exception->what()).data());
		}
		else
		{
			response.setBody(message.data());
		}
#endif

		response.setResponseCode(web::ResponseCodes::badRequest);
	}

	void ResourceExecutor::forbiddenError(HTTPResponseExecutors& response, const exception* exception)
	{
		string_view message = HTMLErrorsData[HTMLErrors::forbidden403];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(format("{} Exception: {}", message, exception->what()).data());
		}
		else
		{
			response.setBody(message.data());
		}
#endif

		response.setResponseCode(web::ResponseCodes::forbidden);
	}

	void ResourceExecutor::internalServerError(HTTPResponseExecutors& response, const exception* exception)
	{
		string_view message = HTMLErrorsData[HTMLErrors::internalServerError500];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(format("{} Exception: {}", message, exception->what()).data());
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
		string_view message = HTMLErrorsData[HTMLErrors::badGateway502];

#ifdef NDEBUG
		response.setBody(message);
#else
		if (exception)
		{
			response.setBody(format("{} Exception: {}", message, exception->what()).data());
		}
		else
		{
			response.setBody(message.data());
		}
#endif

		response.setResponseCode(web::ResponseCodes::badGateway);
	}

	bool ResourceExecutor::getIsCaching() const
	{
		return fileManager.getCache().getCacheSize();
	}
}
