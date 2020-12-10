#include "ResourceExecutor.h"

#include <fstream>

#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	void ResourceExecutor::sendAssetFile(HTTPRequest&& request, HTTPResponse& response)
	{
		if (request.getKeyValueParameters().count("Referer"))
		{
			throw exceptions::NotImplementedException();
		}

		string parameters = request.getRawParameters();
		string fileName = parameters.substr(parameters.rfind('/') + 1);
		string result;

		parameters.resize(parameters.rfind('/') + 1);

		filesystem::path filePath(assets.string() + parameters + fileName);

		if (!filesystem::exists(filePath))
		{
			throw exceptions::FileDoesNotExistException();
		}

		ifstream file(filePath);
		string tem;

		while (getline(file, tem))
		{
			result += tem + "\n";
		}

		file.close();

		response.addBody(result);
	}

	ResourceExecutor::ResourceExecutor(const filesystem::path& assets) :
#ifdef WEB_FRAMEWORK_ASSETS
		defaultAssets(WEB_FRAMEWORK_ASSETS),
#else
		defaultAssets(webFrameworkDefaultAssests),
#endif // WEB_FRAMEWORK_ASSETS
		assets(assets)
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
		this->sendAssetFile(move(request), response);
	}

	void ResourceExecutor::doPost(HTTPRequest&& request, HTTPResponse& response)
	{
		this->sendAssetFile(move(request), response);
	}

	void ResourceExecutor::notFoundError(HTTPResponse& response)
	{
		filesystem::path notFoundHTML(defaultAssets);

		notFoundHTML /= "Errors";
		notFoundHTML /= "404.html";

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

		badRequestHTML /= "Errors";
		badRequestHTML /= "400.html";

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

		internalServerErrorHTML /= "Errors";
		internalServerErrorHTML /= "400.html";

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