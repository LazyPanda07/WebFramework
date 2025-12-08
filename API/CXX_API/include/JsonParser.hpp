#pragma once

#include <ostream>

#include "JsonObject.hpp"

namespace framework
{
	class JsonParser
	{
	private:
		void* implementation;

	public:
		JsonParser();

		JsonParser(std::string_view jsonString);

		JsonParser(const JsonParser& other);

		JsonParser(JsonParser&& other) noexcept;

		JsonParser& operator =(const JsonParser& other);

		JsonParser& operator =(JsonParser&& other) noexcept;

		template<JsonValues<JsonObject> T>
		bool contains(std::string_view key, bool recursive = false) const;

		template<typename T>
		void overrideValue(std::string_view key, T&& value, bool recursive = false) requires (JsonValues<T, JsonObject> || std::convertible_to<T, std::string_view> || std::convertible_to<T, std::string>);

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
		using createJSONParser = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONParser, nullptr, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JsonParser::JsonParser(std::string_view jsonString)
	{
		using createJSONParserFromString = void* (*)(const char* jsonString, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONParserFromString, jsonString.data(), &exception);

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
		using createJSONParser = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONParser, other.implementation, &exception);

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
	inline bool JsonParser::contains(std::string_view key, bool recursive) const
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		bool result = false;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonParserBoolean, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonParserBoolean, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonParserNull, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonParserNull, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonParserString, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonParserString, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonParserArray, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonParserArray, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonParserObject, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonParserObject, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonParserDouble, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonParserDouble, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonParserUnsignedInteger, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonParserUnsignedInteger, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonParserInteger, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonParserInteger, key.data(), recursive, &exception);
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

	template<typename T>
	inline void JsonParser::overrideValue(std::string_view key, T&& value, bool recursive) requires (JsonValues<T, JsonObject> || std::convertible_to<T, std::string_view> || std::convertible_to<T, std::string>)
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideJsonParserBoolean, void, const char* key, bool value, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideJsonParserBoolean, key.data(), value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideJsonParserNull, void, const char* key, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideJsonParserNull, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideJsonParserString, void, const char* key, const char* value, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideJsonParserString, key.data(), value.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideJsonParserArray, void, const char* key, void** value, size_t size, bool recursive, void** exception);
			std::vector<void*> temp;

			temp.reserve(value.size());

			for (const JsonObject& object : value)
			{
				temp.push_back(object.implementation);
			}

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideJsonParserArray, key.data(), temp.data(), temp.size(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideJsonParserObject, void, const char* key, const void* value, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideJsonParserObject, key.data(), value, recursive, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideJsonParserDouble, void, const char* key, double value, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideJsonParserDouble, key.data(), value, recursive, &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideJsonParserUnsignedInteger, void, const char* key, uint64_t value, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideJsonParserUnsignedInteger, key.data(), value, recursive, &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(overrideJsonParserInteger, void, const char* key, int64_t value, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(overrideJsonParserInteger, key.data(), value, recursive, &exception);
		}
		else
		{
			throw std::invalid_argument(std::format("Wrong argument type: {}", typeid(T).name()));
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	template<JsonValues<JsonObject> T>
	inline T JsonParser::get(std::string_view key, bool recursive) const requires(!std::convertible_to<T, std::string_view> || std::same_as<T, std::string>)
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		T result;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserBoolean, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserBoolean, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserNull, void, const char* key, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserNull, key.data(), recursive, &exception);

			result = nullptr;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserString, const char*, const char* key, bool recursive, void** exception);

			result = std::string(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserString, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserArray, void, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserArray, key.data(), &JsonObject::addArrayValue, &result, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserObject, void*, const char* key, bool recursive, void** exception);

			result = JsonObject(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserObject, key.data(), recursive, &exception), false);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserDouble, double, const char* key, bool recursive, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserDouble, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserUnsignedInteger, uint64_t, const char* key, bool recursive, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserUnsignedInteger, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserInteger, int64_t, const char* key, bool recursive, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserInteger, key.data(), recursive, &exception));
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
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserBoolean, bool, const char* key, bool* value, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserBoolean, key.data(), &value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserNull, void, const char* key, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserNull, key.data(), recursive, &exception);

			result = true;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserString, bool, const char* key, void** value, bool recursive, void** exception);
			void* string = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserString, key.data(), &string, recursive, &exception))
			{
				value = handler.getString(string);

				result = true;
			}
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserArray, bool, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserArray, key.data(), &JsonObject::addArrayValue, &value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserObject, bool, const char* key, void** value, bool recursive, void** exception);
			void* object = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserObject, key.data(), &object, recursive, &exception))
			{
				value = JsonObject(object, false);

				result = true;
			}
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserDouble, bool, const char* key, double* value, bool recursive, void** exception);
			double temp = 0.0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserDouble, key.data(), &temp, recursive, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserUnsignedInteger, bool, const char* key, uint64_t * value, bool recursive, void** exception);
			uint64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserUnsignedInteger, key.data(), &temp, recursive, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserInteger, bool, const char* key, int64_t * value, bool recursive, void** exception);
			int64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserInteger, key.data(), &temp, recursive, &exception))
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
		DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserRawData, const char*, void** exception);
		void* exception = nullptr;

		std::string_view result = utility::DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserRawData, &exception);

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
			utility::DLLHandler::getInstance().deleteJSONParser(implementation);

			implementation = nullptr;
		}
	}
}
