#pragma once

#include <unordered_map>

#include "DatabaseInterfaces/ISQLResult.hpp"

#include "SQLValue.hpp"

namespace framework
{
	class SqlResult
	{
	public:
		using Rows = std::vector<std::unordered_map<std::string, SqlValue>>;
		using Row = std::unordered_map<std::string, SqlValue>;

	private:
		Rows rows;

	private:
		static void reserveSize(size_t size, void* buffer);

		static void fill(const char** columnNames, const void** values, size_t size, size_t index, void* buffer);

	private:
		SqlResult(interfaces::ISQLResult* implementation);

	public:
		/**
		 * @brief Returns a reference to the row at the specified index.
		 * @param index Zero-based index of the row to access.
		 * @return A reference to the Row at the given index. The returned reference can be used to modify the row. Behavior is undefined if the index is out of range.
		 */
		Row& at(size_t index);

		/**
		 * @brief Returns a const reference to the Row at the specified index.
		 * @param index Zero-based index of the row to access.
		 * @return A const reference to the Row at the given index (read-only).
		 */
		const Row& at(size_t index) const;

		/**
		 * @brief Returns the number of elements in the object. This is a const member function and does not modify the object.
		 * @return The number of elements (size) as a size_t.
		 */
		size_t size() const;

		/**
		 * @brief Returns an iterator to the first element in the Rows collection.
		 * @return A Rows::iterator pointing to the first element; if the collection is empty, returns end().
		 */
		Rows::iterator begin();

		/**
		 * @brief Returns an iterator to the element following the last element in the Rows container.
		 * @return A Rows::iterator pointing past the last element (the end iterator).
		 */
		Rows::iterator end();

		/**
		 * @brief Conversion operator that yields a reference to a Rows object, allowing implicit conversion to Rows&.
		 */
		operator Rows& ();

		/**
		 * @brief Converts the object to a const reference to its Rows representation without modifying the object.
		 */
		operator const Rows& () const;

		/**
		 * @brief Provides mutable access to the row at the given index.
		 * @param index The zero-based index of the row to access.
		 * @return A reference to the Row at the specified index. Modifying the returned reference modifies the underlying data. Behavior is undefined if index is out of range.
		 */
		Row& operator [](size_t index);

		/**
		 * @brief Returns a const reference to the Row at the specified zero-based index. This const overload does not modify the object.
		 * @param index Zero-based index of the row to retrieve. Must be within the valid range; accessing an out-of-range index is undefined behavior.
		 * @return A const reference to the Row at the specified index. The returned reference cannot be used to modify the row.
		 */
		const Row& operator [](size_t index) const;

		~SqlResult() = default;

		friend class Table;
	};
}

namespace framework
{
	inline void SqlResult::reserveSize(size_t size, void* buffer)
	{
		static_cast<Rows*>(buffer)->resize(size);
	}

	inline void SqlResult::fill(const char** columnNames, const void** values, size_t size, size_t index, void* buffer)
	{
		for (size_t i = 0; i < size; i++)
		{
			(*static_cast<Rows*>(buffer))[index].try_emplace(columnNames[i], SqlValue(static_cast<const interfaces::ISQLValue*>(values[i])));
		}
	}

	inline SqlResult::SqlResult(interfaces::ISQLResult* implementation)
	{
		implementation->iterate(&SqlResult::reserveSize, &SqlResult::fill, &rows);
	}

	inline std::unordered_map<std::string, SqlValue>& SqlResult::at(size_t index)
	{
		return rows.at(index);
	}

	inline const std::unordered_map<std::string, SqlValue>& SqlResult::at(size_t index) const
	{
		return rows.at(index);
	}

	inline size_t SqlResult::size() const
	{
		return rows.size();
	}

	inline SqlResult::Rows::iterator SqlResult::begin()
	{
		return rows.begin();
	}

	inline SqlResult::Rows::iterator SqlResult::end()
	{
		return rows.end();
	}

	inline SqlResult::operator Rows& ()
	{
		return rows;
	}

	inline SqlResult::operator const Rows& () const
	{
		return rows;
	}

	inline SqlResult::Row& SqlResult::operator [](size_t index)
	{
		return rows[index];
	}

	inline const SqlResult::Row& SqlResult::operator [](size_t index) const
	{
		return rows[index];
	}
}
