#include "ResourceExecutor.h"

#include <fstream>

#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	void ResourceExecutor::sendFile(const string& filePath, HTTPResponse& response)
	{
		string result;

		if (isCaching)
		{
			shared_lock<shared_mutex> shared(cacheMutex);

			auto findFile = cache.find(filePath);

			if (findFile != cache.end())
			{
				response.addBody(findFile->second);

				return;
			}
		}

		filesystem::path assetsFilePath(assets.string() + filePath);

		if (!filesystem::exists(assetsFilePath))
		{
			throw exceptions::FileDoesNotExistException();
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
			lock_guard<shared_mutex> insertLock(cacheMutex);

			cache[filePath] = result;
		}

		response.addBody(result);
	}

	ResourceExecutor::ResourceExecutor(const filesystem::path& assets, bool isCaching) :
#ifdef WEB_FRAMEWORK_ASSETS
		defaultAssets(WEB_FRAMEWORK_ASSETS),
#else
		defaultAssets(webFrameworkDefaultAssests),
#endif // WEB_FRAMEWORK_ASSETS
		assets(assets),
		isCaching(isCaching)
	{

	}

	void ResourceExecutor::init(const utility::XMLSettingsParser::ExecutorSettings& settings)
	{
		if (!filesystem::exists(assets))
		{
			filesystem::create_directory(assets);
		}
	}

	void ResourceExecutor::doGet(HTTPRequest&& request, HTTPResponse& response)
	{
		if (!request.getHeaders().count("Referer"))
		{
			throw exceptions::NotImplementedException();
		}

		this->sendFile(request.getRawParameters(), response);
	}

	void ResourceExecutor::doPost(HTTPRequest&& request, HTTPResponse& response)
	{
		if (!request.getHeaders().count("Referer"))
		{
			throw exceptions::NotImplementedException();
		}

		this->sendFile(request.getRawParameters(), response);
	}

	void ResourceExecutor::notFoundError(HTTPResponse& response)
	{
		filesystem::path notFoundHTML(defaultAssets);

		notFoundHTML /= WebFrameworkAssets::errorsFolder;
		notFoundHTML /= WebFrameworkAssets::notFound;

		ifstream errorHTML(notFoundHTML);
		string result;
		string tem;

		while (getline(errorHTML, tem))
		{
			result += tem + "\n";
		}

		errorHTML.close();

		response.setResponseCode(web::ResponseCodes::notFound);

		response.addBody(result);
	}

	void ResourceExecutor::badRequestError(HTTPResponse& response)
	{
		filesystem::path badRequestHTML(defaultAssets);

		badRequestHTML /= WebFrameworkAssets::errorsFolder;
		badRequestHTML /= WebFrameworkAssets::badRequest;

		ifstream errorHTML(badRequestHTML);
		string result;
		string tem;

		while (getline(errorHTML, tem))
		{
			result += tem + "\n";
		}

		errorHTML.close();

		response.setResponseCode(web::ResponseCodes::badRequest);

		response.addBody(result);
	}

	void ResourceExecutor::internalServerError(HTTPResponse& response)
	{
		filesystem::path internalServerErrorHTML(defaultAssets);

		internalServerErrorHTML /= WebFrameworkAssets::errorsFolder;
		internalServerErrorHTML /= WebFrameworkAssets::internalServerError;

		ifstream errorHTML(internalServerErrorHTML);
		string result;
		string tem;

		while (getline(errorHTML, tem))
		{
			result += tem + "\n";
		}

		errorHTML.close();

		response.setResponseCode(web::ResponseCodes::internalServerError);

		response.addBody(result);
	}

	void ResourceExecutor::destroy()
	{

	}
}