#pragma once

#include "JsonObject.hpp"

namespace framework
{
	class JsonBuilder
	{
	private:
		void* implementation;

	public:
		JsonBuilder();

		JsonBuilder(std::string_view jsonString);

		JsonBuilder(const JsonBuilder& other);

		JsonBuilder(JsonBuilder&& other) noexcept;

		JsonBuilder& operator =(const JsonBuilder& other);

		JsonBuilder& operator =(JsonBuilder&& other) noexcept;

		template<JsonValues<JsonObject> T>
		JsonBuilder& append(std::string_view key, const T& value = T());

		template<JsonValues<JsonObject> T>
		bool contains(std::string_view key, bool recursive = false) const;

		std::string build() const;

		void standard();

		void minimize();

		/**
		 * @brief Accesses the Json value associated with the specified key.
		 * @param key The key to look up in the Json object.
		 * @return The JsonObject (returned as weak reference) corresponding to the specified key.
		 */
		JsonObject operator [](std::string_view key);

		explicit operator std::string() const;

		~JsonBuilder();
	};
}

namespace framework
{
	inline JsonBuilder::JsonBuilder()
	{
		using createJsonBuilder = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DllHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJsonBuilder, nullptr, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JsonBuilder::JsonBuilder(std::string_view jsonString)
	{
		using createJsonBuilderFromString = void* (*)(const char* jsonString, void** exception);
		void* exception = nullptr;

		implementation = utility::DllHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJsonBuilderFromString, jsonString.data(), &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JsonBuilder::JsonBuilder(const JsonBuilder& other)
	{
		(*this) = other;
	}

	inline JsonBuilder::JsonBuilder(JsonBuilder&& other) noexcept
	{
		(*this) = std::move(other);
	}

	inline JsonBuilder& JsonBuilder::operator =(const JsonBuilder& other)
	{
		using createJsonBuilder = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DllHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJsonBuilder, other.implementation, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	inline JsonBuilder& JsonBuilder::operator =(JsonBuilder&& other) noexcept
	{
		implementation = other.implementation;

		other.implementation = nullptr;

		return *this;
	}

	template<JsonValues<JsonObject> T>
	inline JsonBuilder& JsonBuilder::append(std::string_view key, const T& value)
	{
		utility::DllHandler& handler = utility::DllHandler::getInstance();
		void* exception = nullptr;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJsonBuilderBoolean, void, const char* key, bool value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderBoolean, key.data(), value, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJsonBuilderNull, void, const char* key, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderNull, key.data(), &exception);
		}
		else if constexpr (std::convertible_to<T, std::string_view>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJsonBuilderString, void, const char* key, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderString, key.data(), static_cast<std::string_view>(value).data(), &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJsonBuilderArray, void, const char* key, void** value, size_t size, void** exception);
			std::vector<void*> temp;

			temp.reserve(value.size());

			for (const JsonObject& object : value)
			{
				temp.push_back(object.implementation);
			}

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderArray, key.data(), temp.data(), temp.size(), &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJsonBuilderObject, void, const char* key, void* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderObject, key.data(), value.implementation, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJsonBuilderDouble, void, const char* key, double value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderDouble, key.data(), static_cast<double>(value), &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJsonBuilderUnsignedInteger, void, const char* key, uint64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderUnsignedInteger, key.data(), static_cast<uint64_t>(value), &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJsonBuilderInteger, void, const char* key, int64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJsonBuilderInteger, key.data(), static_cast<int64_t>(value), &exception);
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

	template<JsonValues<JsonObject> T>
	inline bool JsonBuilder::contains(std::string_view key, bool recursive) const
	{
		utility::DllHandler& handler = utility::DllHandler::getInstance();
		void* exception = nullptr;
		bool result = false;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonBuilderBoolean, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonBuilderBoolean, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonBuilderNull, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonBuilderNull, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonBuilderString, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonBuilderString, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonBuilderArray, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonBuilderArray, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonBuilderObject, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonBuilderObject, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonBuilderDouble, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonBuilderDouble, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonBuilderUnsignedInteger, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonBuilderUnsignedInteger, key.data(), recursive, &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(containsJsonBuilderInteger, bool, const char* key, bool recursive, void** exception);

			result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(containsJsonBuilderInteger, key.data(), recursive, &exception);
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

	inline std::string JsonBuilder::build() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(buildJsonBuilder, void*, void** exception);
		void* exception = nullptr;
		utility::DllHandler& handler = utility::DllHandler::getInstance();

		void* result = handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(buildJsonBuilder, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return handler.getString(result);
	}

	inline void JsonBuilder::standard()
	{
		DEFINE_CLASS_MEMBER_FUNCTION(standardJsonBuilder, void, void** exception);
		void* exception = nullptr;

		utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(standardJsonBuilder, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline void JsonBuilder::minimize()
	{
		DEFINE_CLASS_MEMBER_FUNCTION(minimizeJsonBuilder, void, void** exception);
		void* exception = nullptr;

		utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(minimizeJsonBuilder, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JsonObject JsonBuilder::operator [](std::string_view key)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(accessKeyOperatorJsonBuilder, void*, const char* key, void** exception);
		void* exception = nullptr;
		void* result = utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(accessKeyOperatorJsonBuilder, key.data(), & exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return JsonObject(result);
	}

	inline JsonBuilder::operator std::string() const
	{
		return this->build();
	}

	inline JsonBuilder::~JsonBuilder()
	{
		if (implementation)
		{
			utility::DllHandler::getInstance().deleteJsonBuilder(implementation);

			implementation = nullptr;
		}
	}
}
