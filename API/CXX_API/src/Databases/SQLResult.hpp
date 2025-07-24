#pragma once

#include <unordered_map>

#include "Databases/Interfaces/ISQLResult.h"

#include "SQLValue.hpp"

namespace framework
{
	class SQLResult
	{
	public:
		using ValueType = std::vector<std::unordered_map<std::string, SQLValue>>;

	private:
		ValueType rows;

	private:
		static void fill(const char* columnName, const interfaces::ISQLValue* value, size_t index, size_t size, void* data);

	public:
		SQLResult(interfaces::ISQLResult* implementation);

		std::unordered_map<std::string, SQLValue>& at(size_t index);

		const std::unordered_map<std::string, SQLValue>& at(size_t index) const;

		size_t size() const;

		ValueType::iterator begin();

		ValueType::iterator end();

		operator ValueType& ();

		operator const ValueType& () const;

		std::unordered_map<std::string, SQLValue>& operator[](size_t index);

		const std::unordered_map<std::string, SQLValue>& operator[](size_t index) const;

		~SQLResult() = default;
	};
}

namespace framework
{
	inline void SQLResult::fill(const char* columnName, const interfaces::ISQLValue* value, size_t index, size_t size, void* data)
	{
		ValueType& rows = *static_cast<ValueType*>(data);

		if (rows.empty())
		{
			rows.resize(size);
		}

		rows[index].try_emplace(columnName, SQLValue(value));
	}

	inline SQLResult::SQLResult(interfaces::ISQLResult* implementation)
	{
		implementation->iterate(&SQLResult::fill, &rows);
	}

	inline std::unordered_map<std::string, SQLValue>& SQLResult::at(size_t index)
	{
		return rows.at(index);
	}

	inline const std::unordered_map<std::string, SQLValue>& SQLResult::at(size_t index) const
	{
		return rows.at(index);
	}

	inline size_t SQLResult::size() const
	{
		return rows.size();
	}

	inline SQLResult::ValueType::iterator SQLResult::begin()
	{
		return rows.begin();
	}

	inline SQLResult::ValueType::iterator SQLResult::end()
	{
		return rows.end();
	}

	inline SQLResult::operator ValueType& ()
	{
		return rows;
	}

	inline SQLResult::operator const ValueType& () const
	{
		return rows;
	}

	inline std::unordered_map<std::string, SQLValue>& SQLResult::operator[](size_t index)
	{
		return rows[index];
	}

	inline const std::unordered_map<std::string, SQLValue>& SQLResult::operator[](size_t index) const
	{
		return rows[index];
	}
}
