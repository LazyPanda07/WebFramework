#pragma once

#include "../DLLHandler.hpp"
#include "../Exceptions/WebFrameworkException.hpp"
#include "../Databases/SqlValue.hpp"

namespace framework::utility
{
	namespace uuid
	{
		/**
		 * @brief Generate UUID in format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
		 * @return 
		 */
		std::string generateUUID();
	}
	
	namespace encoding
	{
		/**
		 * @brief Generate SHA256 hash
		 * @param data Data
		 * @return 
		 */
		std::string generateSha256(std::string_view data);
	}

	namespace database
	{
		/**
		 * @brief Helper function for SqlValue
		 * @tparam ...Args 
		 * @param ...args 
		 * @return 
		 */
		template<typename... Args>
		std::array<SqlValue, sizeof...(Args)> makeSQLValues(Args&&... args);
	}

	void generateBinaryAssetFile(const std::filesystem::path& filePath, const std::filesystem::path& outputPath);
}

namespace framework::utility
{
	namespace uuid
	{
		inline std::string generateUUID()
		{
			using generateWebFrameworkUUID = void* (*)(void** exception);
			void* exception = nullptr;
			DllHandler& instance = DllHandler::getInstance();

			void* result = instance.CALL_WEB_FRAMEWORK_FUNCTION(generateWebFrameworkUUID, &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return instance.getString(result);
		}
	}

	namespace encoding
	{
		inline std::string generateSha256(std::string_view data)
		{
			using generateSha256 = void* (*)(const char* data, size_t size, void** exception);
			void* exception = nullptr;
			DllHandler& instance = DllHandler::getInstance();

			void* result = instance.CALL_WEB_FRAMEWORK_FUNCTION(generateSha256, data.data(), data.size(), &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return instance.getString(result);
		}
	}

	namespace database
	{
		template<typename... Args>
		inline std::array<SqlValue, sizeof...(Args)> makeSQLValues(Args&&... args)
		{
			std::array<SqlValue, sizeof...(Args)> result({ std::forward<Args>(args)... });

			return result;
		}
	}

	inline void generateBinaryAssetFile(const std::filesystem::path& filePath, const std::filesystem::path& outputPath)
	{
		using generateBinaryAssetFile = void (*)(const char* filePath, const char* outputPath, void** exception);
		void* exception = nullptr;

		if (!std::filesystem::exists(filePath))
		{
			throw std::runtime_error(std::format("Can't find {} path", filePath.string()));
		}

		if (!std::filesystem::is_directory(filePath))
		{
			throw std::runtime_error(std::format("{} must be directory", filePath.string()));
		}

		DllHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(generateBinaryAssetFile, filePath.string().data(), outputPath.string().data(), &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}
}
