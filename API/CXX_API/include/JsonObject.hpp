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

	class JsonObject
	{
	private:
		void* implementation;
		bool weak;

	public:
		JsonObject();

		/**
		 * @brief Construct view to JsonObject
		 * @param implementation 
		 */
		JsonObject(void* implementation);

		JsonObject(const JsonObject& other);

		JsonObject(JsonObject&& other) noexcept;

		JsonObject& operator =(const JsonObject& other);

		JsonObject& operator =(JsonObject&& other) noexcept;

		template<JsonValues<JsonObject> T>
		void setValue(std::string_view key, const T& value = T());

		~JsonObject();

		friend class JsonBuilder;
		friend class JsonParser;
	};

	namespace utility
	{
		template<JsonValues<JsonObject> T>
		std::vector<JsonObject>& appendArray(std::vector<JsonObject>& data, const T& value);
	}
}

namespace framework
{
	inline JsonObject::JsonObject(void* implementation) :
		implementation(implementation),
		weak(true)
	{

	}

	inline JsonObject::JsonObject() :
		weak(false)
	{
		using createJsonObject = void* (*)(void* object, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJsonObject, nullptr, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JsonObject::JsonObject(const JsonObject& other)
	{
		(*this) = other;
	}

	inline JsonObject::JsonObject(JsonObject&& other) noexcept
	{
		(*this) = std::move(other);
	}

	inline JsonObject& JsonObject::operator =(const JsonObject& other)
	{
		using createJsonObject = void* (*)(void* object, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJsonObject, other.implementation, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		weak = false;

		return *this;
	}

	inline JsonObject& JsonObject::operator =(JsonObject&& other) noexcept
	{
		implementation = other.implementation;
		weak = false;

		other.implementation = nullptr;

		return *this;
	}

	template<JsonValues<JsonObject> T>
	inline void JsonObject::setValue(std::string_view key, const T& value)
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectBoolean, void, const char* key, bool value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectBoolean, key.data(), value, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectNull, void, const char* key, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectNull, key.data(), &exception);
		}
		else if constexpr (std::convertible_to<T, std::string_view>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectString, void, const char* key, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectString, key.data(), static_cast<std::string_view>(value).data(), &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectArray, void, const char* key, const void* value, size_t size, void** exception);
			std::vector<void*> temp;

			temp.reserve(value.size());

			for (const JsonObject& object : value)
			{
				temp.push_back(object.implementation);
			}

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectArray, key.data(), temp.data(), temp.size(), &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectObject, void, const char* key, void* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectObject, key.data(), value.implementation, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectDouble, void, const char* key, double value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectDouble, key.data(), static_cast<double>(value), &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectUnsignedInteger, void, const char* key, uint64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectUnsignedInteger, key.data(), static_cast<uint64_t>(value), &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectInteger, void, const char* key, int64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectInteger, key.data(), static_cast<int64_t>(value), &exception);
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

	inline JsonObject::~JsonObject()
	{
		if (!weak && implementation)
		{
			utility::DLLHandler::getInstance().deleteJsonObject(implementation);

			implementation = nullptr;
		}
	}

	namespace utility
	{
		template<JsonValues<JsonObject> T>
		inline std::vector<JsonObject>& appendArray(std::vector<JsonObject>& data, const T& value)
		{
			JsonObject object;

			object.setValue("", value);

			data.emplace_back(std::move(object));

			return data;
		}
	}
}
