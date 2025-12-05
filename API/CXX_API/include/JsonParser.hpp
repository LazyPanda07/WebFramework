#pragma once

#include <ostream>

#include "JsonObject.hpp"

namespace framework
{
	class JsonParser
	{
	private:
		void* implementation;

	private:
		static void addArrayValue(void* object, void* array)
		{
			static_cast<std::vector<JsonObject>*>(array)->emplace_back(object);
		}

	public:
		JsonParser();

		JsonParser(std::string_view jsonString);

		JsonParser(const JsonParser& other);

		JsonParser(JsonParser&& other) noexcept;

		JsonParser& operator =(const JsonParser& other);

		JsonParser& operator =(JsonParser&& other) noexcept;

		template<JsonValues<JsonObject> T>
		T get(std::string_view key, bool recursive = false) const requires(!std::convertible_to<T, std::string_view> || std::same_as<T, std::string>);

		template<JsonValues<JsonObject> T>
		bool tryGet(std::string_view key, T& value, bool recursive = false) const requires(!std::convertible_to<T, std::string_view> || std::same_as<T, std::string>);

		std::string_view operator *() const;

		friend std::ostream& operator <<(std::ostream& stream, const JsonParser& parser);

		~JsonParser();
	};
}

namespace framework
{
	inline JsonParser::JsonParser()
	{
		using createJsonParser = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJsonParser, nullptr, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JsonParser::JsonParser(std::string_view jsonString)
	{
		using createJsonParserFromString = void* (*)(const char* jsonString, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJsonParserFromString, jsonString.data(), &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JsonParser::JsonParser(const JsonParser& other)
	{
		(*this) = other;
	}

	inline JsonParser::JsonParser(JsonParser&& other) noexcept
	{
		(*this) = std::move(other);
	}

	inline JsonParser& JsonParser::operator =(const JsonParser& other)
	{
		using createJsonParser = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJsonParser, other.implementation, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	inline JsonParser& JsonParser::operator =(JsonParser&& other) noexcept
	{
		implementation = other.implementation;

		other.implementation = nullptr;

		return *this;
	}

	template<JsonValues<JsonObject> T>
	inline T JsonParser::get(std::string_view key, bool recursive) const requires(!std::convertible_to<T, std::string_view> || std::same_as<T, std::string>)
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		T result;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonParserBoolean, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserBoolean, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonParserNull, void, const char* key, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserNull, key.data(), recursive, &exception);

			result = nullptr;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonParserString, const char*, const char* key, bool recursive, void** exception);

			result = std::string(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserString, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonParserArray, void, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserArray, key.data(), &JsonParser::addArrayValue, &result, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonParserObject, void*, const char* key, bool recursive, void** exception);

			result = JsonObject(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserObject, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonParserDouble, double, const char* key, bool recursive, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserDouble, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonParserUnsignedInteger, uint64_t, const char* key, bool recursive, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserUnsignedInteger, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonParserInteger, int64_t, const char* key, bool recursive, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserInteger, key.data(), recursive, &exception));
		}
		else
		{
			throw std::invalid_argument(std::format("Wrong argument type: {}", typeid(T).name()));
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
	}

	template<JsonValues<JsonObject> T>
	inline bool JsonParser::tryGet(std::string_view key, T& value, bool recursive) const requires(!std::convertible_to<T, std::string_view> || std::same_as<T, std::string>)
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		bool result = false;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonParserBoolean, bool, const char* key, bool* value, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserBoolean, key.data(), &value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonParserNull, void, const char* key, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserNull, key.data(), recursive, &exception);

			result = true;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonParserString, bool, const char* key, void** value, bool recursive, void** exception);
			void* string = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserString, key.data(), &string, recursive, &exception))
			{
				value = handler.getString(string);

				result = true;
			}
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonParserArray, bool, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserArray, key.data(), &JsonParser::addArrayValue, &value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonParserObject, bool, const char* key, void** value, bool recursive, void** exception);
			void* object = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserObject, key.data(), &object, recursive, &exception))
			{
				value = JsonObject(object);

				result = true;
			}
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonParserDouble, bool, const char* key, double* value, bool recursive, void** exception);
			double temp = 0.0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserDouble, key.data(), &temp, recursive, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonParserUnsignedInteger, bool, const char* key, uint64_t * value, bool recursive, void** exception);
			uint64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserUnsignedInteger, key.data(), &temp, recursive, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonParserInteger, bool, const char* key, int64_t * value, bool recursive, void** exception);
			int64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonParserInteger, key.data(), &temp, recursive, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else
		{
			throw std::invalid_argument(std::format("Wrong argument type: {}", typeid(T).name()));
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
	}

	inline std::string_view JsonParser::operator *() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(getJsonParserRawData, const char*, void** exception);
		void* exception = nullptr;

		std::string_view result = utility::DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonParserRawData, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
	}

	inline std::ostream& operator <<(std::ostream& stream, const JsonParser& parser)
	{
		return stream << *parser;
	}

	inline JsonParser::~JsonParser()
	{
		if (implementation)
		{
			utility::DLLHandler::getInstance().deleteJsonParser(implementation);

			implementation = nullptr;
		}
	}
}
