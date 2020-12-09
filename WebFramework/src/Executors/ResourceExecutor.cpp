#include "ResourceExecutor.h"

#include <fstream>

#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	void ResourceExecutor::sendAssetFile(HTTPRequest&& request, HTTPResponse& response)
	{
		string parameters = request.getRawParameters();
		string fileName = parameters.substr(parameters.rfind('/')  + 1);
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
		defaultAssets(webFrameworkDefaultAssests),
		assets(assets)
	{

	}

	void ResourceExecutor::init(const utility::XMLSettingsParser::ExecutorSettings& settings)
	{
		if (!filesystem::exists(defaultAssets))
		{
			filesystem::create_directory(defaultAssets);
		}

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

	void ResourceExecutor::destroy()
	{

	}
}