#pragma once

#include "JSONObject.hpp"

namespace framework
{
	class JSONBuilder
	{
	private:
		void* implementation;

	public:
		class JSONBuilderHelper
		{
		private:
			std::string key;
			JSONBuilder& builder;

		public:
			JSONBuilderHelper(std::string_view key, JSONBuilder& builder);

			template<JsonValues<JSONObject> T>
			void operator =(const T& value);

			~JSONBuilderHelper() = default;
		};

	public:
		JSONBuilder();

		JSONBuilder(const JSONBuilder& other);

		JSONBuilder(JSONBuilder&& other) noexcept;

		JSONBuilder& operator =(const JSONBuilder& other);

		JSONBuilder& operator =(JSONBuilder&& other) noexcept;

		template<JsonValues<JSONObject> T>
		JSONBuilder& append(std::string_view key, const T& value = T());

		std::string build() const;

		JSONBuilderHelper operator [](std::string_view key);

		explicit operator std::string() const;

		~JSONBuilder();
	};
}

namespace framework
{
	inline JSONBuilder::JSONBuilderHelper::JSONBuilderHelper(std::string_view key, JSONBuilder& builder) :
		key(key),
		builder(builder)
	{

	}

	template<JsonValues<JSONObject> T>
	inline void JSONBuilder::JSONBuilderHelper::operator =(const T& value)
	{
		builder.append<T>(key, value);
	}

	inline JSONBuilder::JSONBuilder()
	{
		using createJSONBuilder = void* (*)(void* builder, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createJSONBuilder, nullptr, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline JSONBuilder::JSONBuilder(const JSONBuilder& other)
	{
		(*this) = other;
	}

	inline JSONBuilder::JSONBuilder(JSONBuilder&& other) noexcept
	{
		(*this) = std::move(other);
	}

	inline JSONBuilder& JSONBuilder::operator =(const JSONBuilder& other)
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

	inline JSONBuilder& JSONBuilder::operator =(JSONBuilder&& other) noexcept
	{
		implementation = other.implementation;

		other.implementation = nullptr;

		return *this;
	}

	template<JsonValues<JSONObject> T>
	inline JSONBuilder& JSONBuilder::append(std::string_view key, const T& value)
	{
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();
		void* exception = nullptr;

		if constexpr (std::is_same_v<T, bool>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendBoolean, void, const char* key, bool value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendBoolean, key.data(), value, &exception);
		}
		else if constexpr (std::is_same_v<T, std::nullptr_t>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendNull, void, const char* key, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendNull, key.data(), &exception);
		}
		else if constexpr (std::convertible_to<T, std::string_view>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendString, void, const char* key, const char* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendString, key.data(), static_cast<std::string_view>(value).data(), &exception);
		}
		else if constexpr (std::is_same_v<T, std::vector<JSONObject>>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendArray, void, const char* key, const void* value, size_t size, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendArray, key.data(), value.data(), value.size(), &exception);
		}
		else if constexpr (std::is_same_v<T, JSONObject>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendObject, void, const char* key, void* value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendObject, key.data(), value.implementation, &exception);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendDouble, void, const char* key, double value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendDouble, key.data(), static_cast<double>(value), &exception);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendUnsignedInteger, void, const char* key, uint64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendUnsignedInteger, key.data(), static_cast<uint64_t>(value), &exception);
		}
		else if constexpr (std::is_signed_v<T>)
		{
			DEFINE_CLASS_MEMBER_FUNCTION(appendInteger, void, const char* key, int64_t value, void** exception);

			handler.CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendInteger, key.data(), static_cast<int64_t>(value), &exception);
		}
		else
		{
			throw std::invalid_argument("Wrong argument type: " + typeid(T).name());
		}

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return *this;
	}

	inline std::string JSONBuilder::build() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(build, void*, void** exception);
		void* exception = nullptr;
		utility::DLLHandler& handler = utility::DLLHandler::getInstance();

		void* result = utility::DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(build, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return handler.getString(result);
	}

	inline JSONBuilder::JSONBuilderHelper JSONBuilder::operator [](std::string_view key)
	{
		return JSONBuilderHelper(key, *this);
	}

	inline JSONBuilder::operator std::string() const
	{
		return this->build();
	}

	inline JSONBuilder::~JSONBuilder()
	{
		if (implementation)
		{
			utility::DLLHandler::getInstance().deleteJSONBuilder(implementation);

			implementation = nullptr;
		}
	}
}

