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

	void ResourceExecutor::sendStaticFileWithCaching(const string& filePath, HTTPResponse& response)
	{
		string result;
		shared_lock<shared_mutex> shared(cacheMutex);
		auto findFile = staticCache.find(filePath);

		if (findFile != staticCache.end())
		{
			response.addBody(findFile->second);

			return;
		}

		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath.string());
		}

		result = (ostringstream() << ifstream(assetFilePath).rdbuf()).str();

		{
			unique_lock<shared_mutex> insertLock(cacheMutex);

			staticCache[filePath] = result;
		}

		response.addBody(result);
	}

	void ResourceExecutor::sendStaticFileWithoutCaching(const string& filePath, HTTPResponse& response)
	{
		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath.string());
		}

		response.addBody((ostringstream() << ifstream(assetFilePath).rdbuf()).str());
	}

	void ResourceExecutor::sendDynamicFileWithCaching(const string& filePath, HTTPResponse& response, const unordered_map<string_view, string>& variables)
	{
		string result;
		shared_lock<shared_mutex> shared(cacheMutex);
		auto findFile = dynamicCache.find(filePath);

		if (findFile != dynamicCache.end())
		{
			result = findFile->second;

			dynamicPages.run(variables, result);

			response.addBody(result);

			return;
		}

		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath.string());
		}

		result = (ostringstream() << ifstream(assetFilePath).rdbuf()).str();

		{
			unique_lock<shared_mutex> insertLock(cacheMutex);

			dynamicCache[filePath] = result;
		}

		dynamicPages.run(variables, result);

		response.addBody(result);
	}

	void ResourceExecutor::sendDynamicFileWithoutCaching(const string& filePath, HTTPResponse& response, const unordered_map<string_view, string>& variables)
	{
		string result;
		filesystem::path assetFilePath(assets / filePath);

		if (!filesystem::exists(assetFilePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(assetFilePath.string());
		}

		result = (ostringstream() << ifstream(assetFilePath).rdbuf()).str();

		dynamicPages.run(variables, result);

		response.addBody(result);
	}

	ResourceExecutor::ResourceExecutor(const json::JSONParser& configuration, const filesystem::path& assets, bool isCaching, const string& pathToTemplates) :
#ifdef WEB_FRAMEWORK_ASSETS
		defaultAssets(WEB_FRAMEWORK_ASSETS),
#else
		defaultAssets(webFrameworkDefaultAssests),
#endif // WEB_FRAMEWORK_ASSETS
		assets(assets),
		isCaching(isCaching),
		dynamicPages(pathToTemplates),
		fileManager
		(
			file_manager::FileManager::getInstance
			(
				configuration.contains("threadCount", json::utility::variantTypeEnum::jInt64_t) ?
				static_cast<uint32_t>(configuration.get<int64_t>("threadCount")) :
				NULL
			)
		)
	{
		if (isCaching)
		{
			sendStaticFileFunction = bind(&ResourceExecutor::sendStaticFileWithCaching, this, placeholders::_1, placeholders::_2);

			sendDynamicFileFunction = bind(&ResourceExecutor::sendDynamicFileWithCaching, this, placeholders::_1, placeholders::_2, placeholders::_3);
		}
		else
		{
			sendStaticFileFunction = bind(&ResourceExecutor::sendStaticFileWithoutCaching, this, placeholders::_1, placeholders::_2);

			sendDynamicFileFunction = bind(&ResourceExecutor::sendDynamicFileWithoutCaching, this, placeholders::_1, placeholders::_2, placeholders::_3);
		}
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
		sendStaticFileFunction(filePath, response);
	}

	void ResourceExecutor::sendDynamicFile(const string& filePath, HTTPResponse& response, const unordered_map<string_view, string>& variables)
	{
		sendDynamicFileFunction(filePath, response, variables);
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
		return isCaching;
	}
}
