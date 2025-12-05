#pragma once

#include "JsonObject.hpp"

namespace framework
{
	class JsonBuilder
	{
	private:
		void* implementation;

	public:
		class JSONBuilderHelper
		{
		private:
			std::string key;
			JsonBuilder& builder;

		public:
			JSONBuilderHelper(std::string_view key, JsonBuilder& builder);

			template<JsonValues<JsonObject> T>
			void operator =(const T& value);

			~JSONBuilderHelper() = default;
		};

	public:
		JsonBuilder();

		JsonBuilder(std::string_view jsonString);

		JsonBuilder(const JsonBuilder& other);

		JsonBuilder(JsonBuilder&& other) noexcept;

		JsonBuilder& operator =(const JsonBuilder& other);

		JsonBuilder& operator =(JsonBuilder&& other) noexcept;

		template<JsonValues<JsonObject> T>
		JsonBuilder& append(std::string_view key, const T& value = T());

		std::string build() const;

		JSONBuilderHelper operator [](std::string_view key);

		explicit operator std::string() const;

		~JsonBuilder();
	};
}

namespace framework
{
	inline JsonBuilder::JSONBuilderHelper::JSONBuilderHelper(std::string_view key, JsonBuilder& builder) :
		key(key),
		builder(builder)
	{

	}

	template<JsonValues<JsonObject> T>
	inline void JsonBuilder::JSONBuilderHelper::operator =(const T& value)
	{
		builder.append<T>(key, value);
	}

	inline JsonBuilder::JsonBuilder()
	{
		using createJSONBuilder = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONBuilder, nullptr, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JsonBuilder::JsonBuilder(std::string_view jsonString)
	{
		using createJSONBuilderFromString = void* (*)(const char* jsonString, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONBuilderFromString, jsonString.data(), &exception);

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
		using createJSONBuilder = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONBuilder, other.implementation, &exception);

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
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJSONBuilderBoolean, void, const char* key, bool value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderBoolean, key.data(), value, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJSONBuilderNull, void, const char* key, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderNull, key.data(), &exception);
		}
		else if constexpr (std::convertible_to<T, std::string_view>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJSONBuilderString, void, const char* key, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderString, key.data(), static_cast<std::string_view>(value).data(), &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JsonObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJSONBuilderArray, void, const char* key, const void* value, size_t size, void** exception);
			std::vector<void*> temp;

			temp.reserve(value.size());

			for (const JsonObject& object : value)
			{
				temp.push_back(object.implementation);
			}

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderArray, key.data(), temp.data(), temp.size(), &exception);
		}
		else if constexpr (std::is_same_v<T, JsonObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJSONBuilderObject, void, const char* key, void* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderObject, key.data(), value.implementation, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJSONBuilderDouble, void, const char* key, double value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderDouble, key.data(), static_cast<double>(value), &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJSONBuilderUnsignedInteger, void, const char* key, uint64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderUnsignedInteger, key.data(), static_cast<uint64_t>(value), &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendJSONBuilderInteger, void, const char* key, int64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendJSONBuilderInteger, key.data(), static_cast<int64_t>(value), &exception);
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

	inline std::string JsonBuilder::build() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(buildJSONBuilder, void*, void** exception);
		void* exception = nullptr;
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();

		void* result = utility::DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(buildJSONBuilder, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return handler.getString(result);
	}

	inline JsonBuilder::JSONBuilderHelper JsonBuilder::operator [](std::string_view key)
	{
		return JSONBuilderHelper(key, *this);
	}

	inline JsonBuilder::operator std::string() const
	{
		return this->build();
	}

	inline JsonBuilder::~JsonBuilder()
	{
		if (implementation)
		{
			utility::DLLHandler::getInstance().deleteJSONBuilder(implementation);

			implementation = nullptr;
		}
	}
}

