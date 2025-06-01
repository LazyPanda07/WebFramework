#pragma once

#include <variant>

#include "Databases/Interfaces/ISQLValue.h"

#include "ExecutorsConstants.h"

namespace framework
{
	template<typename T>
	concept OneOf = std::is_same_v<T, int64_t> || std::is_same_v<T, double> || std::is_same_v<T, std::string> || std::is_same_v<T, bool> || std::is_same_v<T, std::nullptr_t> || std::is_same_v<T, std::vector<uint8_t>>;

	class EXECUTORS_API SQLValue
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
