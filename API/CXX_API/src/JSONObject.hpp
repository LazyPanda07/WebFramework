#pragma once

#include "DLLHandler.hpp"
#include "WebFrameworkException.hpp"

namespace framework
{
	template<typename T, typename TJsonStruct>
	concept JsonValues = std::integral<T> || std::floating_point<T> || std::same_as<T, std::nullptr_t> || std::same_as<T, std::string> || std::same_as<T, std::vector<TJsonStruct>> || std::same_as<T, TJsonStruct>;

	class JSONObject
	{
	private:
		void* implementation;

	public:
		JSONObject();

		JSONObject(const JSONObject& other);

		JSONObject(JSONObject&& other) noexcept;

		JSONObject& operator =(const JSONObject& other);

		JSONObject& operator =(JSONObject&& other) noexcept;

		template<JsonValues<JSONObject> T>
		void setValue(std::string_view key, const T& value = T());

		~JSONObject();
	};
}

namespace framework
{
	inline JSONObject::JSONObject()
	{
		using createJSONObject = void* (*)(void* object, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONObject, nullptr, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JSONObject::JSONObject(const JSONObject& other)
	{
		(*this) = other;
	}

	inline JSONObject::JSONObject(JSONObject&& other) noexcept
	{
		(*this) = std::move(other);
	}

	inline JSONObject& JSONObject::operator =(const JSONObject& other)
	{
		using createJSONObject = void* (*)(void* object, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONObject, other.implementation, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	inline JSONObject& JSONObject::operator =(JSONObject&& other) noexcept
	{
		implementation = other.implementation;

		other.implementation = nullptr;

		return *this;
	}

	template<JsonValues<JSONObject> T>
	inline void JSONObject::setValue(std::string_view key, const T& value)
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setBoolean, void, const char* key, bool value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setBoolean, key.data(), value, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setNull, void, const char* key, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setNull, key.data(), &exception);
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setString, void, const char* key, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setString, key.data(), value.data(), &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JSONObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setArray, void, const char* key, const void* value, size_t size, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setArray, key.data(), value.data(), value.size(), &exception);
		}
		else if constexpr (std::is_same_v<T, JSONObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setObject, void, const char* key, void* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setObject, key.data(), value.implementation, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setDouble, void, const char* key, double value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setDouble, key.data(), static_cast<double>(value), &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setUnsignedInteger, void, const char* key, uint64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setUnsignedInteger, key.data(), static_cast<uint64_t>(value), &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setInteger, void, const char* key, int64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setInteger, key.data(), static_cast<int64_t>(value), &exception);
		}
		else
		{
			throw std::invalid_argument("Wrong argument type: " + typeid(T).name());
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JSONObject::~JSONObject()
	{
		if (implementation)
		{
			utility::DLLHandler::getInstance().deleteJSONObject(implementation);

			implementation = nullptr;
		}
	}
}
