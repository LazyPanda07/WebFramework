#pragma once

#include "DLLHandler.h"
#include "WebFrameworkException.h"

namespace framework
{
	namespace utility
	{
		class Config
		{
		private:
			std::shared_ptr<DLLHandler> handler;
			void* implementation;
			bool weak;

		public:
			Config(std::shared_ptr<DLLHandler> handler, const std::filesystem::path& configPath);

			Config(std::shared_ptr<DLLHandler> handler, std::string_view serverConfiguration, std::string_view sourcesPath);

			Config(const Config& other);

			Config(Config&& other) noexcept = delete;

			Config& operator = (const Config& other);

			Config& operator = (Config&& other) noexcept = delete;

			Config& overrideConfiguration(std::string_view key, std::string_view value, bool recursive = false);

			Config& overrideConfiguration(std::string_view key, int64_t value, bool recursive = false);

			Config& overrideConfiguration(std::string_view key, bool value, bool recursive = false);

			Config& overrideConfiguration(std::string_view key, const std::vector<std::string>& value, bool recursive = false);

			Config& overrideConfiguration(std::string_view key, const std::vector<int64_t>& value, bool recursive = false);

			std::string_view getConfiguration() const;

			void* getImplementation() const;

			~Config();
		};
	}
}

namespace framework
{
	namespace utility
	{
		inline Config::Config(std::shared_ptr<DLLHandler> handler, const std::filesystem::path& configPath) :
			handler(handler),
			implementation(nullptr),
			weak(false)
		{
			using createConfigFromPath = void* (*)(const char* configPath, void** exception);
			void* exception = nullptr;

			implementation = handler->CALL_FUNCTION(createConfigFromPath, configPath.string().data(), &exception);

			if (exception)
			{
				throw WebFrameworkException(handler, exception);
			}
		}

		inline Config::Config(std::shared_ptr<DLLHandler> handler, std::string_view serverConfiguration, std::string_view sourcesPath) :
			handler(handler),
			implementation(nullptr),
			weak(false)
		{
			using createConfigFromString = void* (*)(const char* serverConfiguration, const char* sourcesPath, void** exception);
			void* exception = nullptr;

			implementation = handler->CALL_FUNCTION(createConfigFromString, serverConfiguration.data(), sourcesPath.data(), &exception);

			if (exception)
			{
				throw WebFrameworkException(handler, exception);
			}
		}

		inline Config::Config(const Config& other)
		{
			(*this) = other;
		}

		inline Config& Config::operator = (const Config& other)
		{
			handler = handler;
			implementation = implementation;
			weak = true;

			return *this;
		}

		inline Config& Config::overrideConfiguration(std::string_view key, std::string_view value, bool recursive)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationString, void, const char* key, const char* value, bool recursive, void** exception);
			void* exception = nullptr;

			handler->CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationString, key.data(), value.data(), recursive, &exception);

			if (exception)
			{
				throw WebFrameworkException(handler, exception);
			}

			return *this;
		}

		inline Config& Config::overrideConfiguration(std::string_view key, int64_t value, bool recursive)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationInteger, void, const char* key, int64_t value, bool recursive, void** exception);
			void* exception = nullptr;

			handler->CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationInteger, key.data(), value, recursive, &exception);

			if (exception)
			{
				throw WebFrameworkException(handler, exception);
			}

			return *this;
		}

		inline Config& Config::overrideConfiguration(std::string_view key, bool value, bool recursive)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideConfigurationBoolean, void, const char* key, bool value, bool recursive, void** exception);
			void* exception = nullptr;

			handler->CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationBoolean, key.data(), value, recursive, &exception);

			if (exception)
			{
				throw WebFrameworkException(handler, exception);
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

			handler->CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationStringArray, key.data(), data, recursive, static_cast<int64_t>(value.size()), &exception);

			delete[] data;

			if (exception)
			{
				throw WebFrameworkException(handler, exception);
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

			handler->CALL_CLASS_MEMBER_FUNCTION(overrideConfigurationIntegerArray, key.data(), data, recursive, static_cast<int64_t>(value.size()), &exception);

			delete[] data;

			if (exception)
			{
				throw WebFrameworkException(handler, exception);
			}

			return *this;
		}

		inline std::string_view Config::getConfiguration() const
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getConfiguration, const char*, void** exception);
			void* exception = nullptr;

			const char* result = handler->CALL_CLASS_MEMBER_FUNCTION(getConfiguration, &exception);

			if (exception)
			{
				throw WebFrameworkException(handler, exception);
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
				handler->free(implementation);
			}
		}
	}
}
