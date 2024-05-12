#include "ResourceExecutor.h"

#include <fstream>

#include "WebFramework.h"
#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/BadRequestException.h"

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

		HTMLErrorsData[HTMLErrors::notFound404] = readFile(html);

		html.open(allErrorsFolder / web_framework_assets::internalServerError);

		HTMLErrorsData[HTMLErrors::internalServerError500] = readFile(html);
	}

	void ResourceExecutor::readFile(string& result, unique_ptr<file_manager::ReadFileHandle>&& handle)
	{
		result = handle->readAllData();

		if (result.empty())
		{
			throw exceptions::BadRequestException("File is empty");
		}
	}

	ResourceExecutor::ResourceExecutor(const json::JSONParser& configuration, const filesystem::path& assets, uint64_t cachingSize, const string& pathToTemplates) :
		defaultAssets
		(
			configuration.getObject(json_settings::webFrameworkObject).contains(json_settings::webFrameworkDefaultAssetsPath, json::utility::variantTypeEnum::jString) ? 
			configuration.getObject(json_settings::webFrameworkObject).getString(json_settings::webFrameworkDefaultAssetsPath) :
			webFrameworkDefaultAssests
		),
		assets(assets),
		dynamicPages(pathToTemplates),
		fileManager(file_manager::FileManager::getInstance())
	{
		fileManager.getCache().setCacheSize(cachingSize);
	}

	void ResourceExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{
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

	void ResourceExecutor::sendStaticFile(const string& filePath, HTTPResponse& response, bool isBinary, const string& fileName)
	{
		string result;
		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		if (isBinary)
		{
			fileManager.readBinaryFile(assetFilePath, bind(&ResourceExecutor::readFile, this, ref(result), placeholders::_1));
		}
		else
		{
			fileManager.readFile(assetFilePath, bind(&ResourceExecutor::readFile, this, ref(result), placeholders::_1));
		}

		if (fileName.size())
		{
			response.addHeader("Content-Disposition", format(R"(attachment; filename="{}")", fileName));
		}

		response.addBody(move(result));
	}

	void ResourceExecutor::sendDynamicFile(const string& filePath, HTTPResponse& response, const unordered_map<string, string>& variables, bool isBinary, const string& fileName)
	{
		string result;
		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		if (isBinary)
		{
			fileManager.readBinaryFile(assetFilePath, bind(&ResourceExecutor::readFile, this, ref(result), placeholders::_1));
		}
		else
		{
			fileManager.readFile(assetFilePath, bind(&ResourceExecutor::readFile, this, ref(result), placeholders::_1));
		}

		dynamicPages.run(variables, result);

		if (fileName.size())
		{
			response.addHeader("Content-Disposition", format(R"(attachment; filename="{}")", fileName));
		}

		response.addBody(move(result));
	}

	void ResourceExecutor::registerDynamicFunction(const string& functionName, function<string(const vector<string>&)>&& function)
	{
		dynamicPages.registerDynamicFunction(functionName, move(function));
	}

	void ResourceExecutor::unregisterDynamicFunction(const string& functionName)
	{
		dynamicPages.unregisterDynamicFunction(functionName);
	}

	bool ResourceExecutor::isDynamicFunctionRegistered(const string& functionName)
	{
		return dynamicPages.isDynamicFunctionRegistered(functionName);
	}

	void ResourceExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		if (!request.getHeaders().count("Referer"))
		{
			throw exceptions::NotImplementedException();
		}

		request.sendAssetFile(request.getRawParameters(), response);
	}

	void ResourceExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		if (!request.getHeaders().count("Referer"))
		{
			throw exceptions::NotImplementedException();
		}

		request.sendAssetFile(request.getRawParameters(), response);
	}

	const filesystem::path& ResourceExecutor::getPathToAssets() const
	{
		return assets;
	}

	void ResourceExecutor::notFoundError(HTTPResponse& response, const exception* exception)
	{
		const string& message = HTMLErrorsData[HTMLErrors::notFound404];

#ifdef NDEBUG
		response.addBody(message);
#else
		if (exception)
		{
			response.addBody(format("{} Exception: {}", message, exception->what()));
		}
		else
		{
			response.addBody(message);
		}
#endif

		response.setResponseCode(web::responseCodes::notFound);
	}

	void ResourceExecutor::badRequestError(HTTPResponse& response, const exception* exception)
	{
		const string& message = HTMLErrorsData[HTMLErrors::badRequest400];

#ifdef NDEBUG
		response.addBody(message);
#else
		if (exception)
		{
			response.addBody(format("{} Exception: {}", message, exception->what()));
		}
		else
		{
			response.addBody(message);
		}
#endif

		response.setResponseCode(web::responseCodes::badRequest);
	}

	void ResourceExecutor::internalServerError(HTTPResponse& response, const exception* exception)
	{		
		const string& message = HTMLErrorsData[HTMLErrors::internalServerError500];

#ifdef NDEBUG
		response.addBody(message);
#else
		if (exception)
		{
			response.addBody(format("{} Exception: {}", message, exception->what()));
		}
		else
		{
			response.addBody(message);
		}
#endif

		response.setResponseCode(web::responseCodes::internalServerError);
	}

	bool ResourceExecutor::getIsCaching() const
	{
		return fileManager.getCache().getCacheSize();
	}
}
