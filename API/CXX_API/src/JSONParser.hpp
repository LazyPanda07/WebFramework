#pragma once

#include "JSONObject.hpp"

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
			DEFINE_CLASS_MEMBER_FUNCTION(getBoolean, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getBoolean, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getNull, void, const char* key, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getNull, key.data(), recursive, &exception);

			result = nullptr;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getString, const char*, const char* key, bool recursive, void** exception);

			result = std::string(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getString, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_same_v<T, std::vector<JSONObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getArray, void, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getArray, key.data(), &JSONParser::addArrayValue, &result, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JSONObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getObject, void*, const char* key, bool recursive, void** exception);

			result = JSONObject(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getObject, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getDouble, double, const char* key, bool recursive, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDouble, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getUnsignedInteger, uint64_t, const char* key, bool recursive, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getUnsignedInteger, key.data(), recursive, &exception));
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getInteger, int64_t, const char* key, bool recursive, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getInteger, key.data(), recursive, &exception));
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
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetBoolean, bool, const char* key, bool* value, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetBoolean, key.data(), &value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetNull, void, const char* key, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetNull, key.data(), recursive, &exception);

			result = true;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetString, bool, const char* key, void** value, bool recursive, void** exception);
			void* string = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetString, key.data(), &string, recursive, &exception))
			{
				value = handler.getString(string);

				result = true;
			}
		}
		else if constexpr (std::is_same_v<T, std::vector<JSONObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetArray, bool, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetArray, key.data(), &JSONParser::addArrayValue, &value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JSONObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetObject, bool, const char* key, void** value, bool recursive, void** exception);
			void* object = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetObject, key.data(), &object, recursive, &exception))
			{
				value = JSONObject(object);

				result = true;
			}
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetDouble, bool, const char* key, double* value, bool recursive, void** exception);
			double temp = 0.0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetDouble, key.data(), &temp, recursive, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetUnsignedInteger, bool, const char* key, uint64_t * value, bool recursive, void** exception);
			uint64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetUnsignedInteger, key.data(), &temp, recursive, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetInteger, bool, const char* key, int64_t * value, bool recursive, void** exception);
			int64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetInteger, key.data(), &temp, recursive, &exception))
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

	inline JSONParser::~JSONParser()
	{
		if (implementation)
		{
			utility::DLLHandler::getInstance().deleteJSONParser(implementation);

			implementation = nullptr;
		}
	}
}
