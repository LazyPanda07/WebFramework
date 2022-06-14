#include "ResourceExecutor.h"

#include <fstream>

#include "WebFramework.h"
#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	void ResourceExecutor::loadHTMLErrorsData()
	{
		const filesystem::path allErrorsFolder(defaultAssets / web_framework_assets::errorsFolder);
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
	}

	ResourceExecutor::ResourceExecutor(const json::JSONParser& configuration, const filesystem::path& assets, uint64_t cachingSize, const string& pathToTemplates) :
#ifdef WEB_FRAMEWORK_ASSETS
		defaultAssets(WEB_FRAMEWORK_ASSETS),
#else
		defaultAssets(webFrameworkDefaultAssests),
#endif // WEB_FRAMEWORK_ASSETS
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

	void ResourceExecutor::sendStaticFile(const string& filePath, HTTPResponse& response)
	{
		string result;
		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		fileManager.readFile(assetFilePath, bind(&ResourceExecutor::readFile, this, result, placeholders::_1));

		response.addBody(move(result));
	}

	void ResourceExecutor::sendDynamicFile(const string& filePath, HTTPResponse& response, const unordered_map<string_view, string>& variables)
	{
		string result;
		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath);
		}

		fileManager.readFile(assetFilePath, bind(&ResourceExecutor::readFile, this, result, placeholders::_1));

		dynamicPages.run(variables, result);

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

	void ResourceExecutor::doGet(HTTPRequest&& request, HTTPResponse& response)
	{
		if (!request.getHeaders().count("Referer"))
		{
			throw exceptions::NotImplementedException();
		}

		request.sendAssetFile(request.getRawParameters(), response);
	}

	void ResourceExecutor::doPost(HTTPRequest&& request, HTTPResponse& response)
	{
		if (!request.getHeaders().count("Referer"))
		{
			throw exceptions::NotImplementedException();
		}

		request.sendAssetFile(request.getRawParameters(), response);
	}

	void ResourceExecutor::notFoundError(HTTPResponse& response)
	{
		response.setResponseCode(web::responseCodes::notFound);

		response.addBody(HTMLErrorsData[HTMLErrors::notFound404]);
	}

	void ResourceExecutor::badRequestError(HTTPResponse& response)
	{
		response.setResponseCode(web::responseCodes::badRequest);

		response.addBody(HTMLErrorsData[HTMLErrors::badRequest400]);
	}

	void ResourceExecutor::internalServerError(HTTPResponse& response)
	{
		response.setResponseCode(web::responseCodes::internalServerError);

		response.addBody(HTMLErrorsData[HTMLErrors::internalServerError500]);
	}

	bool ResourceExecutor::getIsCaching() const
	{
		return fileManager.getCache().getCacheSize();
	}
}
