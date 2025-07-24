#pragma once

#include <cstdint>
#include <format>
#include <vector>

#include "DLLHandler.hpp"
#include "Exceptions/WebFrameworkException.hpp"

namespace framework
{
	template<typename T, typename TJsonStruct>
	concept JsonValues = std::integral<T> || std::floating_point<T> || std::same_as<T, std::nullptr_t> || std::convertible_to<T, std::string_view> || std::same_as<T, std::vector<TJsonStruct>> || std::same_as<T, TJsonStruct>;

	class JSONObject
	{
	private:
		void* implementation;
		bool weak;

	public:
		JSONObject();

		/**
		 * @brief Construct view to JSONObject
		 * @param implementation 
		 */
		JSONObject(void* implementation);

		JSONObject(const JSONObject& other);

		JSONObject(JSONObject&& other) noexcept;

		JSONObject& operator =(const JSONObject& other);

		JSONObject& operator =(JSONObject&& other) noexcept;

		template<JsonValues<JSONObject> T>
		void setValue(std::string_view key, const T& value = T());

		~JSONObject();

		friend class JSONBuilder;
		friend class JSONParser;
	};

	namespace utility
	{
		template<JsonValues<JSONObject> T>
		std::vector<JSONObject>& appendArray(std::vector<JSONObject>& data, const T& value);
	}
}

namespace framework
{
	inline JSONObject::JSONObject(void* implementation) :
		implementation(implementation),
		weak(true)
	{

	}

	inline JSONObject::JSONObject() :
		weak(false)
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

		weak = false;

		return *this;
	}

	inline JSONObject& JSONObject::operator =(JSONObject&& other) noexcept
	{
		implementation = other.implementation;
		weak = false;

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
		else if constexpr (std::convertible_to<T, std::string_view>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setString, void, const char* key, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setString, key.data(), static_cast<std::string_view>(value).data(), &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JSONObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setArray, void, const char* key, const void* value, size_t size, void** exception);
			std::vector<void*> temp;

			temp.reserve(value.size());

			for (const JSONObject& object : value)
			{
				temp.push_back(object.implementation);
			}

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setArray, key.data(), temp.data(), temp.size(), &exception);
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
			throw std::invalid_argument(std::format("Wrong argument type: {}", typeid(T).name()));
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JSONObject::~JSONObject()
	{
		if (!weak && implementation)
		{
			utility::DLLHandler::getInstance().deleteJSONObject(implementation);

			implementation = nullptr;
		}
	}

	namespace utility
	{
		template<JsonValues<JSONObject> T>
		inline std::vector<JSONObject>& appendArray(std::vector<JSONObject>& data, const T& value)
		{
			JSONObject object;

			object.setValue("", value);

			data.emplace_back(std::move(object));

			return data;
		}
	}
}
