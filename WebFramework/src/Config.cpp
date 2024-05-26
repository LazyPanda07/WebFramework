#include "Config.h"

#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		Config Config::createConfig(string_view serverConfiguration, string_view sourcesPath)
		{
			Config result;

			result.currentConfiguration.setJSONData(serverConfiguration);
			result.basePath = filesystem::absolute(sourcesPath);

			return result;
		}

		Config::Config(const filesystem::path& configPath) :
			basePath(filesystem::absolute(configPath))
		{
			if (!filesystem::exists(configPath))
			{
				throw file_manager::exceptions::FileDoesNotExistException(configPath.string());
			}

			basePath.remove_filename();

			currentConfiguration.setJSONData(ifstream(configPath));
		}

		Config& Config::overrideConfiguration(string_view key, const json::utility::jsonObject::variantType& value, bool recursive)
		{
			currentConfiguration.overrideValue(key, value, recursive);

			return *this;
		}

		const filesystem::path& Config::getBasePath() const
		{
			return basePath;
		}

		const json::JSONParser& Config::operator * () const
		{
			return currentConfiguration;
		}
	}
}
