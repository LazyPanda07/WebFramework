#pragma once

#include <unordered_map>

#include "DatabaseInterfaces/ISQLResult.h"

#include "SQLValue.hpp"

namespace framework
{
	class SQLResult
	{
	public:
		using Rows = std::vector<std::unordered_map<std::string, SQLValue>>;
		using Row = std::unordered_map<std::string, SQLValue>;

	private:
		Rows rows;

	private:
		static void reserveSize(size_t size, void* buffer);

		static void fill(const char** columnNames, const void** values, size_t size, size_t index, void* buffer);

	public:
		SQLResult(interfaces::ISQLResult* implementation);

		std::unordered_map<std::string, SQLValue>& at(size_t index);

		const std::unordered_map<std::string, SQLValue>& at(size_t index) const;

		size_t size() const;

		Rows::iterator begin();

		Rows::iterator end();

		operator Rows& ();

		operator const Rows& () const;

		Row& operator [](size_t index);

		const Row& operator [](size_t index) const;

		~SQLResult() = default;
	};
}

namespace framework
{
	inline void SQLResult::reserveSize(size_t size, void* buffer)
	{
		static_cast<Rows*>(buffer)->resize(size);
	}

	inline void SQLResult::fill(const char** columnNames, const void** values, size_t size, size_t index, void* buffer)
	{
		for (size_t i = 0; i < size; i++)
		{
			(*static_cast<Rows*>(buffer))[index].try_emplace(columnNames[i], SQLValue(static_cast<const interfaces::ISQLValue*>(values[i])));
		}
	}

	inline SQLResult::SQLResult(interfaces::ISQLResult* implementation)
	{
		implementation->iterate(&SQLResult::reserveSize, &SQLResult::fill, &rows);
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

	inline SQLResult::Rows::iterator SQLResult::begin()
	{
		return rows.begin();
	}

	inline SQLResult::Rows::iterator SQLResult::end()
	{
		return rows.end();
	}

	inline SQLResult::operator Rows& ()
	{
		return rows;
	}

	inline SQLResult::operator const Rows& () const
	{
		return rows;
	}

	inline SQLResult::Row& SQLResult::operator [](size_t index)
	{
		return rows[index];
	}

	inline const SQLResult::Row& SQLResult::operator [](size_t index) const
	{
		return rows[index];
	}
}
