#pragma once

#include <variant>
#include <vector>
#include <string>
#include <stdexcept>

#include "DatabaseInterfaces/ISQLValue.hpp"

namespace framework
{
	template<typename T>
	concept OneOf = std::same_as<T, int64_t> || std::same_as<T, double> || std::same_as<T, std::string> || std::same_as<T, bool> || std::same_as<T, std::nullptr_t> || std::same_as<T, std::vector<uint8_t>>;

	class SqlValue
	{
	public:
		using ValueType = std::variant<int64_t, double, std::string, bool, std::nullptr_t, std::vector<uint8_t>>;

	private:
		ValueType value;

	private:
		explicit SqlValue(const interfaces::ISQLValue* implementation);

	public:
		template<typename... Args>
		SqlValue(Args&&... args) requires std::constructible_from<ValueType, Args...>;

		SqlValue(size_t value);

		template<OneOf T>
		const T& get() const;

		const ValueType& operator *() const;

		~SqlValue() = default;

		friend class SqlResult;
	};

	template<OneOf T>
	const T& SqlValue::get() const
	{
		return std::get<T>(value);
	}
}

namespace framework
{
	inline SqlValue::SqlValue(const interfaces::ISQLValue* implementation)
	{
		switch (implementation->getType())
		{
		case interfaces::ISQLValue::SQLValueType::intType:
			value = implementation->getInt();

			break;

		case interfaces::ISQLValue::SQLValueType::doubleType:
			value = implementation->getDouble();

			break;

		case interfaces::ISQLValue::SQLValueType::stringType:
			value = implementation->getString();

			break;

		case interfaces::ISQLValue::SQLValueType::boolType:
			value = implementation->getBool();

			break;

		case interfaces::ISQLValue::SQLValueType::nullptrType:
			value = nullptr;

			break;

		case interfaces::ISQLValue::SQLValueType::blobType:
		{
			size_t size = 0;
			const uint8_t* data = implementation->getBlob(&size);

			value = std::vector<uint8_t>(data, data + size);
		}

		break;

		default:
			throw std::runtime_error("Wrong type from framework::interfaces::ISQLValue::SQLValueType");
		}
	}

	template<typename... Args>
	inline SqlValue::SqlValue(Args&&... args) requires std::constructible_from<ValueType, Args...> :
		value(std::forward<Args>(args)...)
	{

	}

	inline SqlValue::SqlValue(size_t value) :
		value(static_cast<int64_t>(value))
	{

	}

	inline const SqlValue::ValueType& SqlValue::operator *() const
	{
		return value;
	}
}
