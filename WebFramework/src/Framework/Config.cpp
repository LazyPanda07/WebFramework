#include "Config.h"

#include <fstream>

#include "Exceptions/FileDoesNotExistException.h"

namespace framework::utility
{
	template<std::ranges::range T>
	Config& Config::overrideConfigurationArray(std::string_view key, const T& value, bool recursive)
	{
		std::vector<json::utility::jsonObject> data;

		data.reserve(distance(value.begin(), value.end()));

		for (const auto& temp : value)
		{
			json::utility::appendArray(temp, data);
		}

		currentConfiguration.overrideValue(key, data, recursive);

		return *this;
	}

	Config::Config(const std::filesystem::path& configPath)
	{
		if (!std::filesystem::exists(configPath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(configPath.string());
		}

		basePath = std::filesystem::absolute(configPath).parent_path();

		currentConfiguration.setJSONData(std::ifstream(configPath));
	}

	Config::Config(std::string_view serverConfiguration, std::string_view applicationDirectory) :
		currentConfiguration(serverConfiguration),
		basePath(std::filesystem::absolute(applicationDirectory))
	{

	}

	Config& Config::overrideConfiguration(std::string_view key, const json::utility::jsonObject::variantType& value, bool recursive)
	{
		currentConfiguration.overrideValue(key, value, recursive);

		return *this;
	}

	Config& Config::overrideConfiguration(std::string_view key, const std::vector<int64_t>& value, bool recursive)
	{
		return this->overrideConfigurationArray(key, value, recursive);
	}

	Config& Config::overrideConfiguration(std::string_view key, const std::vector<std::string>& value, bool recursive)
	{
		return this->overrideConfigurationArray(key, value, recursive);
	}

	Config& Config::overrideBasePath(const std::filesystem::path& basePath) //-V688
	{
		if (!std::filesystem::exists(basePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(basePath.string());
		}

		this->basePath = basePath;

		return *this;
	}

	const std::string& Config::getConfigurationString(std::string_view key, bool recursive) const
	{
		return currentConfiguration.getString(key, recursive);
	}

	int64_t Config::getConfigurationInteger(std::string_view key, bool recursive) const
	{
		return currentConfiguration.getInt(key, recursive);
	}

	bool Config::getConfigurationBoolean(std::string_view key, bool recursive) const
	{
		return currentConfiguration.getBool(key, recursive);
	}

	const std::filesystem::path& Config::getBasePath() const
	{
		return basePath;
	}

	std::string Config::getConfiguration() const
	{
		return (std::ostringstream() << currentConfiguration).str();
	}

	std::string_view Config::getRawConfiguration() const
	{
		return currentConfiguration.getRawData();
	}

	const json::JSONParser& Config::operator * () const
	{
		return currentConfiguration;
	}
}
