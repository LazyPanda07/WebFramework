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

	private:
		JsonObject(void* implementation);

	public:
		JsonObject();

		JsonObject(const JsonObject& other);

		JsonObject(JsonObject&& other) noexcept;

		JsonObject& operator =(const JsonObject& other);

		JsonObject& operator =(JsonObject&& other) noexcept;

		JsonObject operator [](size_t index);

		JsonObject operator [](std::string_view key);

		template<JsonValues<JsonObject> T>
		JsonObject emplace_back(T&& value);

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
		using createJSONObject = void* (*)(void* object, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONObject, nullptr, &exception);

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

	inline JsonObject& JsonObject::operator =(JsonObject&& other) noexcept
	{
		implementation = other.implementation;
		weak = other.weak;

		other.implementation = nullptr;

		return *this;
	}

	inline JsonObject JsonObject::operator [](size_t index)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(accessIndexOperatorJsonObject, void*, size_t index, void** exception);
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		void* result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(accessIndexOperatorJsonObject, index, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return JsonObject(result);
	}

	inline JsonObject JsonObject::operator [](std::string_view key)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(accessKeyOperatorJsonObject, void*, const char* key, void** exception);
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		void* result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(accessKeyOperatorJsonObject, key.data(), &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return JsonObject(result);
	}

	template<JsonValues<JsonObject> T>
	JsonObject JsonObject::emplace_back(T&& value)
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		void* result = nullptr;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackBoolean, void*, bool value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackBoolean, value, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackNull, void*, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackNull, &exception);
		}
		else if constexpr (std::convertible_to<T, std::string_view>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackString, void*, const char* value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackString, static_cast<std::string_view>(value).data(), &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackArray, void*, const void* value, size_t size, void** exception);
			std::vector<void*> temp;

			temp.reserve(value.size());

			for (const JsonObject& object : value)
			{
				temp.push_back(object.implementation);
			}

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackArray, temp.data(), temp.size(), &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackObject, void*, void* value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackObject, value.implementation, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackDouble, void*, double value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackDouble, static_cast<double>(value), &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackUnsignedInteger, void*, uint64_t value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackUnsignedInteger, static_cast<uint64_t>(value), &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackInteger, void*, int64_t value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackInteger, static_cast<int64_t>(value), &exception);
		}
		else
		{
			throw std::invalid_argument(std::format("Wrong argument type: {}", typeid(T).name()));
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return JsonObject(result);
	}

	template<JsonValues<JsonObject> T>
	inline void JsonObject::setValue(std::string_view key, const T& value)
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJSONObjectBoolean, void, const char* key, bool value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectBoolean, key.data(), value, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJSONObjectNull, void, const char* key, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectNull, key.data(), &exception);
		}
		else if constexpr (std::convertible_to<T, std::string_view>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJSONObjectString, void, const char* key, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectString, key.data(), static_cast<std::string_view>(value).data(), &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJSONObjectArray, void, const char* key, const void* value, size_t size, void** exception);
			std::vector<void*> temp;

			temp.reserve(value.size());

			for (const JsonObject& object : value)
			{
				temp.push_back(object.implementation);
			}

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectArray, key.data(), temp.data(), temp.size(), &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJSONObjectObject, void, const char* key, void* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectObject, key.data(), value.implementation, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJSONObjectDouble, void, const char* key, double value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectDouble, key.data(), static_cast<double>(value), &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJSONObjectUnsignedInteger, void, const char* key, uint64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectUnsignedInteger, key.data(), static_cast<uint64_t>(value), &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJSONObjectInteger, void, const char* key, int64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJSONObjectInteger, key.data(), static_cast<int64_t>(value), &exception);
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
			utility::DLLHandler::getInstance().deleteJSONObject(implementation);

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
