#include "ResourceExecutor.h"

#include <fstream>

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

			return result;
		};

		HTMLErrorsData[HTMLErrors::badRequest400] = readFile(html);
		html.close();

		html.open(allErrorsFolder / web_framework_assets::notFound);
		HTMLErrorsData[HTMLErrors::notFound404] = readFile(html);
		html.close();

		html.open(allErrorsFolder / web_framework_assets::internalServerError);
		HTMLErrorsData[HTMLErrors::internalServerError500] = readFile(html);
		html.close();
	}

	ResourceExecutor::ResourceExecutor(const filesystem::path& assets, bool isCaching, const string& pathToTemplates) :
#ifdef WEB_FRAMEWORK_ASSETS
		defaultAssets(WEB_FRAMEWORK_ASSETS),
#else
		defaultAssets(webFrameworkDefaultAssests),
#endif // WEB_FRAMEWORK_ASSETS
		assets(assets),
		isCaching(isCaching),
		dynamicPages(pathToTemplates)
	{

	}

	void ResourceExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{
		if (!filesystem::exists(assets))
		{
			filesystem::create_directory(assets);
		}

		if (!filesystem::exists(dynamicPages.getPathToTemplates()))
		{
			filesystem::create_directory(dynamicPages.getPathToTemplates());
		}

		this->loadHTMLErrorsData();
	}

	void ResourceExecutor::sendStaticFile(const string& filePath, HTTPResponse& response)
	{
		string result;

		if (isCaching)
		{
			shared_lock<shared_mutex> shared(cacheMutex);

			auto findFile = staticCache.find(filePath);

			if (findFile != staticCache.end())
			{
				response.addBody(findFile->second);

				return;
			}
		}

		filesystem::path assetsFilePath(assets.string() + filePath);

		if (!filesystem::exists(assetsFilePath))
		{
			throw exceptions::FileDoesNotExistException(assetsFilePath.string());
		}

		ifstream file(assetsFilePath);
		string tem;

		while (getline(file, tem))
		{
			result += tem + "\n";
		}

		file.close();

		if (isCaching)
		{
			unique_lock<shared_mutex> insertLock(cacheMutex);

			staticCache[filePath] = result;
		}

		response.addBody(result);
	}

	void ResourceExecutor::sendDynamicFile(const string& filePath, HTTPResponse& response, const smartPointer<unordered_map<string_view, string>>& variables)
	{
		string result;

		if (isCaching)
		{
			shared_lock<shared_mutex> shared(cacheMutex);

			auto findFile = dynamicCache.find(filePath);

			if (findFile != dynamicCache.end())
			{
				result = findFile->second;

				dynamicPages.run(variables, result);

				response.addBody(result);

				return;
			}
		}

		filesystem::path assetsFilePath(assets.string() + filePath);

		if (!filesystem::exists(assetsFilePath))
		{
			throw exceptions::FileDoesNotExistException(assetsFilePath.string());
		}

		ifstream file(assetsFilePath);
		string tem;

		while (getline(file, tem))
		{
			result += tem + "\n";
		}

		file.close();

		if (isCaching)
		{
			unique_lock<shared_mutex> insertLock(cacheMutex);

			dynamicCache[filePath] = result;
		}

		dynamicPages.run(variables, result);

		response.addBody(result);
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
		response.setResponseCode(web::ResponseCodes::notFound);

		response.addBody(HTMLErrorsData[HTMLErrors::notFound404]);
	}

	void ResourceExecutor::badRequestError(HTTPResponse& response)
	{
		response.setResponseCode(web::ResponseCodes::badRequest);

		response.addBody(HTMLErrorsData[HTMLErrors::badRequest400]);
	}

	void ResourceExecutor::internalServerError(HTTPResponse& response)
	{
		response.setResponseCode(web::ResponseCodes::internalServerError);

		response.addBody(HTMLErrorsData[HTMLErrors::internalServerError500]);
	}
}
