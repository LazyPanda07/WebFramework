#pragma once

#include <concepts>

#include "DLLHandler.hpp"
#include "Exceptions/WebFrameworkException.hpp"

namespace framework::utility
{
	/**
	 * @brief Config file representation
	 */
	class Config
	{
	private:
		void* implementation;
		bool weak;

	public:
		/**
		 * @brief
		 * @param configPath Path to *.json config file
		 */
		Config(const std::filesystem::path& configPath);

		/**
		 * @brief
		 * @param serverConfiguration *.json config file content
		 * @param applicationDirectory Working directory
		 */
		Config(std::string_view serverConfiguration, std::string_view applicationDirectory);

		Config(const Config& other);

		Config(Config&& other) noexcept = delete;

		Config& operator = (const Config& other);

		Config& operator = (Config&& other) noexcept = delete;

		/**
		 * @brief Override string
		 * @tparam T
		 * @param key Json key
		 * @param value New string value
		 * @param recursive Recursive search for key
		 * @return Self
		 */
		template<std::convertible_to<std::string_view> T>
		Config& overrideConfiguration(std::string_view key, const T& value, bool recursive = true);

		/**
		 * @brief Override integer
		 * @tparam T
		 * @param key Json key
		 * @param value New integer value
		 * @param recursive Recursive search for key
		 * @return Self
		 */
		template<std::convertible_to<int64_t> T>
		Config& overrideConfiguration(std::string_view key, const T& value, bool recursive = true);

		/**
		 * @brief Override string array
		 * @param key Json key
		 * @param value New string array value
		 * @param recursive Recursive search for key
		 * @return Self
		 */
		Config& overrideConfiguration(std::string_view key, const std::vector<std::string>& value, bool recursive = true);

		/**
		 * @brief Override integer array
		 * @param key Json key
		 * @param value New integer array value
		 * @param recursive Recursive search for key
		 * @return Self
		 */
		Config& overrideConfiguration(std::string_view key, const std::vector<int64_t>& value, bool recursive = true);

		/**
		 * @brief Override config file directory
		 * @param basePath New base path
		 * @return Self
		 */
		Config& overrideBasePath(std::string_view basePath);

		/**
		 * @brief Get value from config
		 * @tparam T 
		 * @param key Config key
		 * @param recursive Search recursively
		 * @return Config value
		 */
		template<typename T>
		T get(std::string_view key, bool recursive = true) const requires(std::same_as<T, std::string> || std::convertible_to<T, int64_t> || std::same_as<T, bool>);

		/**
		 * @brief Get config file directory
		 * @return
		 */
		std::string getBasePath() const;

		/**
		 * @brief Get current config Json string data
		 * @return
		 */
		std::string getConfiguration() const;

		/**
		 * @brief Get raw config Json string data
		 * @return
		 */
		std::string_view getRawConfiguration() const;

		void* getImplementation() const;

		~Config();
	};
}

namespace framework::utility
{
	inline Config::Config(const std::filesystem::path& configPath) :
		implementation(nullptr),
		weak(false)
	{
		using createConfigFromPath = void* (*)(const char* configPath, void** exception);

		if (!std::filesystem::exists(configPath))
		{
			throw std::runtime_error(std::format("Path {} doesn't exist", configPath.string()));
		}

		void* exception = nullptr;

		implementation = DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createConfigFromPath, configPath.string().data(), &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline Config::Config(std::string_view serverConfiguration, std::string_view applicationDirectory) :
		implementation(nullptr),
		weak(false)
	{
		using createConfigFromString = void* (*)(const char* serverConfiguration, const char* applicationDirectory, void** exception);
		void* exception = nullptr;

		implementation = DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createConfigFromString, serverConfiguration.data(), applicationDirectory.data(), &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline Config::Config(const Config& other)
	{
		(*this) = other;
	}

	inline Config& Config::operator = (const Config& other)
	{
		implementation = other.implementation;
		weak = true;

		return *this;
	}

	template<std::convertible_to<std::string_view> T>
	inline Config& Config::overrideConfiguration(std::string_view key, const T& value, bool recursive)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationString, void, const char* key, const char* value, bool recursive, void** exception);
		void* exception = nullptr;

		DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationString, key.data(), static_cast<std::string_view>(value).data(), recursive, &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	template<std::convertible_to<int64_t> T>
	inline Config& Config::overrideConfiguration(std::string_view key, const T& value, bool recursive)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationInteger, void, const char* key, int64_t value, bool recursive, void** exception);
		void* exception = nullptr;

		DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationInteger, key.data(), static_cast<int64_t>(value), recursive, &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	template<>
	inline Config& Config::overrideConfiguration<bool>(std::string_view key, const bool& value, bool recursive) //-V835
	{
		DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationBoolean, void, const char* key, bool value, bool recursive, void** exception);
		void* exception = nullptr;

		DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationBoolean, key.data(), value, recursive, &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	inline Config& Config::overrideConfiguration(std::string_view key, const std::vector<std::string>& value, bool recursive)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationStringArray, void, const char* key, const char** value, bool recursive, int64_t size, void** exception);
		void* exception = nullptr;

		const char** data = new const char* [value.size()];

#ifndef __LINUX__
#pragma warning(push)
#pragma warning(disable: 6386)
#endif

		for (size_t i = 0; i < value.size(); i++)
		{
			data[i] = value[i].data();
		}

#ifndef __LINUX__
#pragma warning(pop)
#endif

		DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationStringArray, key.data(), data, recursive, static_cast<int64_t>(value.size()), &exception);

		delete[] data;

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	inline Config& Config::overrideConfiguration(std::string_view key, const std::vector<int64_t>& value, bool recursive)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationIntegerArray, void, const char* key, const int64_t * value, bool recursive, int64_t size, void** exception);
		void* exception = nullptr;

		int64_t* data = new int64_t[value.size()];

#ifndef __LINUX__
#pragma warning(push)
#pragma warning(disable: 6386)
#endif

		for (size_t i = 0; i < value.size(); i++)
		{
			data[i] = value[i];
		}

#ifndef __LINUX__
#pragma warning(pop)
#endif

		DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideConfigurationIntegerArray, key.data(), data, recursive, static_cast<int64_t>(value.size()), &exception);

		delete[] data;

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	inline Config& Config::overrideBasePath(std::string_view basePath)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(overrideBasePath, void, const char* basePath, void** exception);
		void* exception = nullptr;

		DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideBasePath, basePath.data(), &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	template<typename T>
	inline T Config::get(std::string_view key, bool recursive) const requires(std::same_as<T, std::string> || std::convertible_to<T, int64_t> || std::same_as<T, bool>)
	{
		void* exception = nullptr;
		DLLHandler& instance = DLLHandler::getInstance();
		T result;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getConfigurationBoolean, bool, const char* key, bool recursive, void** exception);

			result = instance.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationBoolean, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_convertible_v<T, int64_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getConfigurationInteger, int64_t, const char* key, bool recursive, void** exception);

			result = static_cast<T>(instance.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationInteger, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getConfigurationString, void*, const char* key, bool recursive, void** exception);

			void* temp = instance.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfigurationString, key.data(), recursive, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return instance.getString(temp);
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
	}

	inline std::string Config::getBasePath() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(getBasePath, void*, void** exception);
		void* exception = nullptr;
		DLLHandler& handler = DLLHandler::getInstance();

		void* result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getBasePath, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return handler.getString(result);
	}

	inline std::string Config::getConfiguration() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(getConfiguration, void*, void** exception);
		void* exception = nullptr;
		DLLHandler& handler = DLLHandler::getInstance();

		void* result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getConfiguration, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return handler.getString(result);
	}

	inline std::string_view Config::getRawConfiguration() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(getRawConfiguration, const char*, void** exception);
		void* exception = nullptr;

		const char* result = DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRawConfiguration, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return std::string_view(result);
	}

	inline void* Config::getImplementation() const
	{
		return implementation;
	}

	inline Config::~Config()
	{
		if (!weak)
		{
			DLLHandler::getInstance().deleteConfig(implementation);
		}
	}
}
