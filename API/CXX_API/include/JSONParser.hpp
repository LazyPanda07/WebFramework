#pragma once

#include <ostream>

#include "JsonObject.hpp"

namespace framework
{
	class JSONParser
	{
	private:
		void* implementation;

	private:
		static void addArrayValue(void* object, void* array)
		{
			static_cast<std::vector<JSONObject>*>(array)->emplace_back(object);
		}

	public:
		JSONParser();

		JSONParser(std::string_view jsonString);

		JSONParser(const JSONParser& other);

		JSONParser(JSONParser&& other) noexcept;

		JSONParser& operator =(const JSONParser& other);

		JSONParser& operator =(JSONParser&& other) noexcept;

		template<JsonValues<JSONObject> T>
		T get(std::string_view key, bool recursive = false) const requires(!std::convertible_to<T, std::string_view> || std::same_as<T, std::string>);

		template<JsonValues<JSONObject> T>
		bool tryGet(std::string_view key, T& value, bool recursive = false) const requires(!std::convertible_to<T, std::string_view> || std::same_as<T, std::string>);

		std::string_view operator *() const;

		friend std::ostream& operator <<(std::ostream& stream, const JSONParser& parser);

		~JSONParser();
	};
}

namespace framework
{
	inline JSONParser::JSONParser()
	{
		using createJSONParser = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONParser, nullptr, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JSONParser::JSONParser(std::string_view jsonString)
	{
		using createJSONParserFromString = void* (*)(const char* jsonString, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONParserFromString, jsonString.data(), &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JSONParser::JSONParser(const JSONParser& other)
	{
		(*this) = other;
	}

	inline JSONParser::JSONParser(JSONParser&& other) noexcept
	{
		(*this) = std::move(other);
	}

	inline JSONParser& JSONParser::operator =(const JSONParser& other)
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

	inline JSONParser& JSONParser::operator =(JSONParser&& other) noexcept
	{
		implementation = other.implementation;

		other.implementation = nullptr;

		return *this;
	}

	template<JsonValues<JSONObject> T>
	inline T JSONParser::get(std::string_view key, bool recursive) const requires(!std::convertible_to<T, std::string_view> || std::same_as<T, std::string>)
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
		else if constexpr (std::is_same_v<T, std::vector<JSONObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserArray, void, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserArray, key.data(), &JSONParser::addArrayValue, &result, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JSONObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJSONParserObject, void*, const char* key, bool recursive, void** exception);

			result = JSONObject(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJSONParserObject, key.data(), recursive, &exception));
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

	template<JsonValues<JSONObject> T>
	inline bool JSONParser::tryGet(std::string_view key, T& value, bool recursive) const requires(!std::convertible_to<T, std::string_view> || std::same_as<T, std::string>)
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
		else if constexpr (std::is_same_v<T, std::vector<JSONObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserArray, bool, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserArray, key.data(), &JSONParser::addArrayValue, &value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JSONObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJSONParserObject, bool, const char* key, void** value, bool recursive, void** exception);
			void* object = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJSONParserObject, key.data(), &object, recursive, &exception))
			{
				value = JSONObject(object);

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

	inline std::string_view JSONParser::operator *() const
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

	inline std::ostream& operator <<(std::ostream& stream, const JSONParser& parser)
	{
		return stream << *parser;
	}

	inline JSONParser::~JSONParser()
	{
		if (implementation)
		{
			utility::DLLHandler::getInstance().deleteJSONParser(implementation);

			implementation = nullptr;
		}
	}
}
