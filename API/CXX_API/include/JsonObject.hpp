#pragma once

#include <cstdint>
#include <format>
#include <vector>

#include "DLLHandler.hpp"
#include "Exceptions/WebFrameworkException.hpp"

namespace framework
{
	template<typename T, typename TJsonStruct>
	concept JsonValues =
		std::integral<std::remove_cvref_t<T>> ||
		std::floating_point<std::remove_cvref_t<T>> ||
		std::same_as<std::remove_cvref_t<T>, std::nullptr_t> ||
		std::convertible_to<std::remove_cvref_t<T>, std::string_view> ||
		std::same_as<std::remove_cvref_t<T>, std::vector<TJsonStruct>> ||
		std::same_as<std::remove_cvref_t<T>, TJsonStruct>;

	class JsonObject
	{
	private:
		void* implementation;
		bool weak;

	private:
		static void addArrayValue(void* object, void* array);

	private:
		JsonObject(void* implementation, bool weak = true);

	public:
		JsonObject();

		JsonObject(const JsonObject& other);

		JsonObject(JsonObject&& other) noexcept;

		JsonObject& operator =(const JsonObject& other);

		JsonObject& operator =(JsonObject&& other) noexcept;

		size_t size() const;

		/**
		 * @brief Returns the Json element at the specified index.
		 * @param index The zero-based index of the element to access.
		 * @return A JsonObject (returned as weak reference) representing the element at the given index.
		 */
		JsonObject operator [](size_t index);

		template<JsonValues<JsonObject> T>
		JsonObject emplace_back(T&& value);

		template<JsonValues<JsonObject> T>
		bool is() const;

		template<JsonValues<JsonObject> T>
		bool contains(std::string_view key, bool recursive = false) const;

		template<JsonValues<JsonObject> T>
		T get() const;

		template<JsonValues<JsonObject> T>
		bool tryGet(std::string_view key, T& value, bool recursive = false) const;

		template<JsonValues<JsonObject> T>
		bool tryGet(T& value) const;

		/**
		 * @brief Accesses the Json value associated with the specified key.
		 * @param key The key to look up in the Json object.
		 * @return The JsonObject (returned as weak reference) corresponding to the specified key.
		 */
		template<typename T>
		JsonObject operator [](T&& key) requires(std::convertible_to<T, std::string_view> || std::same_as<T, std::string>);

		template<typename T>
		JsonObject& operator =(T&& value) requires (JsonValues<T, JsonObject> || std::convertible_to<T, std::string_view> || std::convertible_to<T, std::string>);

		friend std::ostream& operator <<(std::ostream& stream, const JsonObject& object);

		~JsonObject();

		friend class JsonBuilder;
		friend class JsonParser;
	};
}

namespace framework
{
	inline void JsonObject::addArrayValue(void* object, void* array)
	{
		static_cast<std::vector<JsonObject>*>(array)->push_back(JsonObject(object));
	}

	inline JsonObject::JsonObject(void* implementation, bool weak) :
		implementation(implementation),
		weak(weak)
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
		weak = other.weak;

		other.implementation = nullptr;

		return *this;
	}

	inline size_t JsonObject::size() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(sizeJsonObject, size_t, void** exception);
		void* exception = nullptr;

		size_t result = utility::DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sizeJsonObject, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
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

	template<JsonValues<JsonObject> T>
	JsonObject JsonObject::emplace_back(T&& value)
	{
		using ActualT = std::remove_cvref_t<T>;

		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		void* result = nullptr;

		if constexpr (std::is_same_v<ActualT, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackBoolean, void*, bool value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackBoolean, value, &exception);
		}
		else if constexpr (std::is_same_v<ActualT, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackNull, void*, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackNull, &exception);
		}
		else if constexpr (std::convertible_to<ActualT, std::string_view>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackString, void*, const char* value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackString, static_cast<std::string_view>(value).data(), &exception);
		}
		else if constexpr (std::is_same_v<ActualT, std::vector<JsonObject>>)
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
		else if constexpr (std::is_same_v<ActualT, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackObject, void*, void* value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackObject, value.implementation, &exception);
		}
		else if constexpr (std::is_floating_point_v<ActualT>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackDouble, void*, double value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackDouble, static_cast<double>(value), &exception);
		}
		else if constexpr (std::is_unsigned_v<ActualT>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(emplaceBackUnsignedInteger, void*, uint64_t value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(emplaceBackUnsignedInteger, static_cast<uint64_t>(value), &exception);
		}
		else if constexpr (std::is_signed_v<ActualT>)
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
	inline bool JsonObject::is() const
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		bool result = false;

		if constexpr (std::same_as<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(isObject, bool, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isObject, &exception);
		}
		else if constexpr (std::same_as<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(isString, bool, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isString, &exception);
		}
		else if constexpr (std::same_as<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(isBoolean, bool, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isBoolean, &exception);
		}
		else if constexpr (std::floating_point<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(isDouble, bool, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isDouble, &exception);
		}
		else if constexpr (std::signed_integral<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(isInteger, bool, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isInteger, &exception);
		}
		else if constexpr (std::unsigned_integral<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(isUnsignedInteger, bool, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isUnsignedInteger, &exception);
		}
		else if constexpr (std::same_as<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(isNull, bool, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isNull, &exception);
		}
		else if constexpr (std::same_as<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(isArray, bool, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isArray, &exception);
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
	}

	template<JsonValues<JsonObject> T>
	inline bool JsonObject::contains(std::string_view key, bool recursive) const
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		bool result = false;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonObjectBoolean, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonObjectBoolean, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonObjectNull, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonObjectNull, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonObjectString, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonObjectString, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonObjectArray, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonObjectArray, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonObjectObject, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonObjectObject, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonObjectDouble, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonObjectDouble, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonObjectUnsignedInteger, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonObjectUnsignedInteger, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonObjectInteger, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonObjectInteger, key.data(), recursive, &exception);
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
	inline T JsonObject::get() const
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		T result;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonObjectBoolean, bool, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectBoolean, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonObjectNull, void, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectNull, &exception);

			result = nullptr;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonObjectString, const char*, void** exception);

			result = std::string(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectString, &exception));
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonObjectArray, void, void(*addArrayValue)(void* object, void* array), void* array, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectArray, &JsonObject::addArrayValue, &result, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonObjectObject, void*, void** exception);

			result = JsonObject(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectObject, &exception), false);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonObjectDouble, double, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectDouble, &exception));
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonObjectUnsignedInteger, uint64_t, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectUnsignedInteger, &exception));
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(getJsonObjectInteger, int64_t, void** exception);

			result = static_cast<T>(handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getJsonObjectInteger, &exception));
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
	inline bool JsonObject::tryGet(std::string_view key, T& value, bool recursive) const
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		bool result = false;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectBooleanByKey, bool, const char* key, bool* value, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectBooleanByKey, key.data(), &value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectNull, void, const char* key, bool recursive, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectNull, key.data(), recursive, &exception);

			result = true;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectStringByKey, bool, const char* key, void** value, bool recursive, void** exception);
			void* string = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectStringByKey, key.data(), &string, recursive, &exception))
			{
				value = handler.getString(string);

				result = true;
			}
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectArrayByKey, bool, const char* key, void(*addArrayValue)(void* object, void* array), void* array, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectArrayByKey, key.data(), &JsonObject::addArrayValue, &value, recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectObjectByKey, bool, const char* key, void** value, bool recursive, void** exception);
			void* object = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectObjectByKey, key.data(), &object, recursive, &exception))
			{
				value = JsonObject(object, false);

				result = true;
			}
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectDoubleByKey, bool, const char* key, double* value, bool recursive, void** exception);
			double temp = 0.0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectDoubleByKey, key.data(), &temp, recursive, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectUnsignedIntegerByKey, bool, const char* key, uint64_t * value, bool recursive, void** exception);
			uint64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectUnsignedIntegerByKey, key.data(), &temp, recursive, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectIntegerByKey, bool, const char* key, int64_t * value, bool recursive, void** exception);
			int64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectIntegerByKey, key.data(), &temp, recursive, &exception))
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

	template<JsonValues<JsonObject> T>
	inline bool JsonObject::tryGet(T& value) const
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		bool result = false;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectBoolean, bool, bool* value, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectBoolean, &value, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectNull, void, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectNull, &exception);

			result = true;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectString, bool, void** value, void** exception);
			void* string = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectString, &string, &exception))
			{
				value = handler.getString(string);

				result = true;
			}
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectArray, bool, void(*addArrayValue)(void* object, void* array), void* array, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectArray, &JsonObject::addArrayValue, &value, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectObject, bool, void** value, void** exception);
			void* object = nullptr;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectObject, &object, &exception))
			{
				value = JsonObject(object, false);

				result = true;
			}
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectDouble, bool, double* value, void** exception);
			double temp = 0.0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectDouble, &temp, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectUnsignedInteger, bool, uint64_t* value, void** exception);
			uint64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectUnsignedInteger, &temp, &exception))
			{
				value = static_cast<T>(temp);

				result = true;
			}
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(tryGetJsonObjectInteger, bool, int64_t* value, void** exception);
			int64_t temp = 0;

			if (handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(tryGetJsonObjectInteger, &temp, &exception))
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

	template<typename T>
	inline JsonObject JsonObject::operator [](T&& key) requires(std::convertible_to<T, std::string_view> || std::same_as<T, std::string>)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(accessKeyOperatorJsonObject, void*, const char* key, void** exception);
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;
		void* result = nullptr;

		if constexpr (std::same_as<T, std::string>)
		{
			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(accessKeyOperatorJsonObject, key.data(), &exception);
		}
		else if constexpr (std::convertible_to<T, std::string_view>)
		{
			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(accessKeyOperatorJsonObject, static_cast<std::string_view>(key).data(), &exception);
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return JsonObject(result);
	}

	template<typename T>
	inline JsonObject& JsonObject::operator =(T&& value) requires (JsonValues<T, JsonObject> || std::convertible_to<T, std::string_view> || std::convertible_to<T, std::string>)
	{
		using ActualT = std::remove_cvref_t<T>;

		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;

		if constexpr (std::is_same_v<ActualT, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectBoolean, void, bool value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectBoolean, value, &exception);
		}
		else if constexpr (std::is_same_v<ActualT, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectNull, void, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectNull, &exception);
		}
		else if constexpr (std::same_as<ActualT, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectString, void, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectString, value.data(), &exception);
		}
		else if constexpr (std::convertible_to<ActualT, std::string_view>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectString, void, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectString, static_cast<std::string_view>(value).data(), &exception);
		}
		else if constexpr (std::convertible_to<ActualT, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectString, void, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectString, static_cast<std::string>(value).data(), &exception);
		}
		else if constexpr (std::is_same_v<ActualT, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectArray, void, const void* value, size_t size, void** exception);
			std::vector<void*> temp;

			temp.reserve(value.size());

			for (const JsonObject& object : value)
			{
				temp.push_back(object.implementation);
			}

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectArray, temp.data(), temp.size(), &exception);
		}
		else if constexpr (std::is_same_v<ActualT, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectObject, void, void* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectObject, value.implementation, &exception);
		}
		else if constexpr (std::is_floating_point_v<ActualT>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectDouble, void, double value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectDouble, static_cast<double>(value), &exception);
		}
		else if constexpr (std::is_unsigned_v<ActualT>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectUnsignedInteger, void, uint64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectUnsignedInteger, static_cast<uint64_t>(value), &exception);
		}
		else if constexpr (std::is_signed_v<ActualT>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(setJsonObjectInteger, void, int64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setJsonObjectInteger, static_cast<int64_t>(value), &exception);
		}
		else
		{
			throw std::invalid_argument(std::format("Wrong argument type: {}", typeid(T).name()));
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	inline std::ostream& operator <<(std::ostream& stream, const JsonObject& object)
	{
		using jsonObjectToString = void* (*)(void* implementation, void** exception);
		void* exception = nullptr;
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();

		void* result = handler.CALL_WEB_FRAMEWORK_FUNCTION(jsonObjectToString, object.implementation, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return stream << handler.getString(result);
	}

	inline JsonObject::~JsonObject()
	{
		if (!weak && implementation)
		{
			utility::DLLHandler::getInstance().deleteJsonObject(implementation);

			implementation = nullptr;
		}
	}
}
