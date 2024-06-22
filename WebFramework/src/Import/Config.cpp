#include "Config.h"

#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		template<ranges::range T>
		Config& Config::overrideConfigurationArray(string_view key, const T& value, bool recursive)
		{
			vector<json::utility::jsonObject> data;

			data.reserve(distance(value.begin(), value.end()));

			for (const auto& temp : value)
			{
				json::utility::appendArray(temp, data);
			}

			currentConfiguration.overrideValue(key, data, recursive);

			return *this;
		}

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

		Config& Config::overrideConfiguration(string_view key, const vector<int64_t>& value, bool recursive)
		{
			return this->overrideConfigurationArray(key, value, recursive);
		}

		Config& Config::overrideConfiguration(string_view key, const vector<string>& value, bool recursive)
		{
			return this->overrideConfigurationArray(key, value, recursive);
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