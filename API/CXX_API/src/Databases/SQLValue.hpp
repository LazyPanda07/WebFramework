#pragma once

#include <variant>

#include "DatabaseInterfaces/ISQLValue.h"

namespace framework
{
	template<typename T>
	concept OneOf = std::is_same_v<T, int64_t> || std::is_same_v<T, double> || std::is_same_v<T, std::string> || std::is_same_v<T, bool> || std::is_same_v<T, std::nullptr_t> || std::is_same_v<T, std::vector<uint8_t>>;

	class SQLValue
	{
	public:
		using ValueType = std::variant<int64_t, double, std::string, bool, std::nullptr_t, std::vector<uint8_t>>;

	private:
		ValueType value;

	private:
		SQLValue(const interfaces::ISQLValue* implementation);

	public:
		SQLValue(const ValueType& value);

		SQLValue(ValueType&& value) noexcept;

		template<OneOf T>
		const T& get() const;

		const ValueType& operator *() const;

		~SQLValue() = default;

		friend class SQLResult;
	};

	template<OneOf T>
	const T& SQLValue::get() const
	{
		return std::get<T>(value);
	}
}

namespace framework
{
	inline SQLValue::SQLValue(const interfaces::ISQLValue* implementation)
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

	inline SQLValue::SQLValue(const ValueType& value) :
		value(value)
	{

	}

	inline SQLValue::SQLValue(ValueType&& value) noexcept :
		value(std::move(value))
	{

	}

	inline const SQLValue::ValueType& SQLValue::operator *() const
	{
		return value;
	}
}
