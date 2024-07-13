#pragma once

#include <concepts>

#include "DLLHandler.h"
#include "WebFrameworkException.h"

namespace framework
{
	namespace utility
	{
		class Config
		{
		private:
			void* implementation;
			bool weak;

		public:
			Config(const std::filesystem::path& configPath);

			Config(std::string_view serverConfiguration, std::string_view sourcesPath);

			Config(const Config& other);

			Config(Config&& other) noexcept = delete;

			Config& operator = (const Config& other);

			Config& operator = (Config&& other) noexcept = delete;

			template<std::convertible_to<std::string_view> T>
			Config& overrideConfiguration(std::string_view key, const T& value, bool recursive = false);

			template<std::convertible_to<int64_t> T>
			Config& overrideConfiguration(std::string_view key, const T& value, bool recursive = false);

			template<>
			Config& overrideConfiguration<bool>(std::string_view key, const bool& value, bool recursive);

			Config& overrideConfiguration(std::string_view key, const std::vector<std::string>& value, bool recursive = false);

			Config& overrideConfiguration(std::string_view key, const std::vector<int64_t>& value, bool recursive = false);

			std::string getBasePath() const;

			std::string getConfiguration() const;

			std::string_view getRawConfiguration() const;

			void* getImplementation() const;

			~Config();
		};
	}
}

namespace framework
{
	namespace utility
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

			implementation = DLLHandler::getInstance().CALL_FUNCTION(createConfigFromPath, configPath.string().data(), &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}
		}

		inline Config::Config(std::string_view serverConfiguration, std::string_view sourcesPath) :
			implementation(nullptr),
			weak(false)
		{
			using createConfigFromString = void* (*)(const char* serverConfiguration, const char* sourcesPath, void** exception);
			void* exception = nullptr;

			implementation = DLLHandler::getInstance().CALL_FUNCTION(createConfigFromString, serverConfiguration.data(), sourcesPath.data(), &exception);

			if (exception)
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

			DLLHandler::getInstance().CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationString, key.data(), static_cast<std::string_view>(value).data(), recursive, &exception);

			if (exception)
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

			DLLHandler::getInstance().CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationInteger, key.data(), static_cast<int64_t>(value), recursive, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return *this;
		}

		template<>
		inline Config& Config::overrideConfiguration<bool>(std::string_view key, const bool& value, bool recursive)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationBoolean, void, const char* key, bool value, bool recursive, void** exception);
			void* exception = nullptr;

			DLLHandler::getInstance().CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationBoolean, key.data(), value, recursive, &exception);

			if (exception)
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

			DLLHandler::getInstance().CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationStringArray, key.data(), data, recursive, static_cast<int64_t>(value.size()), &exception);

			delete[] data;

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return *this;
		}

		inline Config& Config::overrideConfiguration(std::string_view key, const std::vector<int64_t>& value, bool recursive)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationIntegerArray, void, const char* key, int64_t * value, bool recursive, int64_t size, void** exception);
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

			DLLHandler::getInstance().CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationIntegerArray, key.data(), data, recursive, static_cast<int64_t>(value.size()), &exception);

			delete[] data;

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return *this;
		}

		inline std::string Config::getBasePath() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getBasePath, void*, void** exception);
			using getDataFromString = const char* (*)(void* implementation);
			void* exception = nullptr;
			DLLHandler& handler = DLLHandler::getInstance();

			void* stringPtr = handler.CALL_CLASS_MEMBER_FUNCTION(getBasePath, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			std::string result(handler.CALL_FUNCTION(getDataFromString, stringPtr));

			handler.free(stringPtr);

			return result;
		}

		inline std::string Config::getConfiguration() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getConfigurationString, void*, void** exception);
			using getDataFromString = const char* (*)(void* implementation);
			void* exception = nullptr;
			DLLHandler& handler = DLLHandler::getInstance();

			void* stringPtr = handler.CALL_CLASS_MEMBER_FUNCTION(getConfigurationString, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			std::string result(handler.CALL_FUNCTION(getDataFromString, stringPtr));

			handler.free(stringPtr);

			return result;
		}

		inline std::string_view Config::getRawConfiguration() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getRawConfiguration, const char*, void** exception);
			void* exception = nullptr;

			const char* result = DLLHandler::getInstance().CALL_CLASS_MEMBER_FUNCTION(getRawConfiguration, &exception);

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
				DLLHandler::getInstance().free(implementation);
			}
		}
	}
}
