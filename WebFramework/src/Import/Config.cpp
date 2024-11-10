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

		Config::Config(const filesystem::path& configPath) :
			basePath(filesystem::absolute(configPath))
		{
			if (!filesystem::exists(configPath))
			{
				throw file_manager::exceptions::FileDoesNotExistException(configPath.string());
			}

			basePath = basePath.parent_path();

			currentConfiguration.setJSONData(ifstream(configPath));
		}

		Config::Config(string_view serverConfiguration, string_view applicationDirectory) :
			currentConfiguration(serverConfiguration),
			basePath(filesystem::absolute(applicationDirectory))
		{

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

		Config& Config::overrideBasePath(const filesystem::path& basePath) //-V688
		{
			if (!filesystem::exists(basePath))
			{
				throw file_manager::exceptions::FileDoesNotExistException(basePath.string());
			}

			this->basePath = basePath;

			return *this;
		}

		const string& Config::getConfigurationString(string_view key, bool recursive) const
		{
			return currentConfiguration.getString(key, recursive);
		}

		int64_t Config::getConfigurationInteger(string_view key, bool recursive) const
		{
			return currentConfiguration.getInt(key, recursive);
		}

		bool Config::getConfigurationBoolean(string_view key, bool recursive) const
		{
			return currentConfiguration.getBool(key, recursive);
		}

		const filesystem::path& Config::getBasePath() const
		{
			return basePath;
		}

		string Config::getConfiguration() const
		{
			return (ostringstream() << currentConfiguration).str();
		}

		string_view Config::getRawConfiguration() const
		{
			return currentConfiguration.getRawData();
		}

		const json::JSONParser& Config::operator * () const
		{
			return currentConfiguration;
		}
	}
}
