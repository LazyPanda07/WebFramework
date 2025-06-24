#pragma once

#include "JSONObject.hpp"

namespace framework
{
	class JSONBuilder
	{
	private:
		void* implementation;

	public:
		JSONBuilder();

		JSONBuilder(const JSONBuilder& other);

		JSONBuilder(JSONBuilder&& other) noexcept;

		JSONBuilder& operator =(const JSONBuilder& other);

		JSONBuilder& operator =(JSONBuilder&& other) noexcept;

		std::string build() const;

		~JSONBuilder();
	};
}

namespace framework
{
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

	inline JSONBuilder::~JSONBuilder()
	{
		if (implementation)
		{
			utility::DLLHandler::getInstance().deleteJSONBuilder(implementation);

			implementation = nullptr;
		}
	}
}

