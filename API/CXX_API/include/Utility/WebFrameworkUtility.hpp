#pragma once

#include <chrono>

#include "../DLLHandler.hpp"
#include "../Exceptions/WebFrameworkException.hpp"
#include "../Databases/SqlValue.hpp"
#include "../JsonObject.hpp"

namespace framework::utility
{
	namespace uuid
	{
		/**
		 * @brief Generate UUID in format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
		 * @return 
		 */
		std::string generateUuid();
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
		std::array<SqlValue, sizeof...(Args)> makeSqlValues(Args&&... args);
	}

	namespace token
	{
		std::string createJwt(const JsonObject& data, std::chrono::minutes expirationTime);
	}

	/**
	 * @brief Generates a binary asset file from the specified input file and writes it to the given output path. Progress may be reported via an optional callback.
	 * @param directoryPath Path to the input directory with assets to convert into a binary asset.
	 * @param outputPath Path where the generated binary asset file will be written.
	 * @param progressCallback Optional callback invoked to report progress. The callback has the signature void(float progress, const char* assetPath, void* data); progress is typically in the range [0.0, 1.0], assetPath is a null-terminated string identifying the currently processed asset, and data is the user pointer provided to this function.
	 * @param data Optional user-defined pointer that is passed through to progressCallback; defaults to nullptr.
	 */
	void generateBinaryAssetFile(const std::filesystem::path& directoryPath, const std::filesystem::path& outputPath, void(*progressCallback)(float progress, const char* assetPath, void* data) = nullptr, void* data = nullptr);
}

namespace framework::utility
{
	namespace uuid
	{
		inline std::string generateUuid()
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
		inline std::array<SqlValue, sizeof...(Args)> makeSqlValues(Args&&... args)
		{
			std::array<SqlValue, sizeof...(Args)> result({ std::forward<Args>(args)... });

			return result;
		}
	}

	namespace token
	{
		inline std::string createJwt(const JsonObject& data, std::chrono::minutes expirationTime)
		{
			using createJwt = void* (*) (void* data, int64_t expirationTimeInMinutes, void** exception);
			void* exception = nullptr;
			DllHandler& instance = DllHandler::getInstance();

			void* result = instance.CALL_WEB_FRAMEWORK_FUNCTION(createJwt, data.implementation, expirationTime.count(), &exception);

			if (exception)
			{
				throw exceptions::WebFrameworkException(exception);
			}

			return instance.getString(result);
		}
	}

	inline void generateBinaryAssetFile(const std::filesystem::path& directoryPath, const std::filesystem::path& outputPath, void(*progressCallback)(float progress, const char* assetPath, void* data), void* data)
	{
		using generateBinaryAssetFile = void (*)(const char* filePath, const char* outputPath, void(*progressCallback)(float progress, const char* assetPath, void* data), void* data, void** exception);
		void* exception = nullptr;

		if (!std::filesystem::exists(directoryPath))
		{
			throw std::runtime_error(std::format("Can't find {} path", directoryPath.string()));
		}

		if (!std::filesystem::is_directory(directoryPath))
		{
			throw std::runtime_error(std::format("{} must be directory", directoryPath.string()));
		}

		DllHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(generateBinaryAssetFile, directoryPath.string().data(), outputPath.string().data(), progressCallback, data, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}
}
