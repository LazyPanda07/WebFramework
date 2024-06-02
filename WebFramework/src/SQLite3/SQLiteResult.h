#pragma once

#include "WebFrameworkCore.h"

namespace framework
{
	namespace sqlite
	{
		namespace utility
		{
			/// <summary>
			/// Contains result of SQL request
			/// <para>Contains all rows after SQL request</para>
			/// </summary>
			class WEB_FRAMEWORK_API SQLiteResult
			{
			public:
				using const_iterator = std::vector<std::unordered_map<std::string, std::string>>::const_iterator;

			private:
				std::vector<std::unordered_map<std::string, std::string>> rows;

			public:
				explicit SQLiteResult(std::vector<std::unordered_map<std::string, std::string>>&& rows) noexcept;

				SQLiteResult(const SQLiteResult& other) = default;

				SQLiteResult(SQLiteResult&& other) noexcept = default;

				SQLiteResult& operator = (const SQLiteResult& other) = default;

				SQLiteResult& operator = (SQLiteResult&& other) noexcept = default;

				/// @brief Number of rows
				/// @return Number of rows
				size_t size() const;

				/// @brief Is SQLiteResult has no rows
				/// @return rows.empty()
				bool isEmpty() const;

				/// @brief Access operator
				/// @param index Column name
				/// @return Column value
				/// @exception std::out_of_range Wrong index
				std::unordered_map<std::string, std::string>& operator [] (size_t index);

				/// @brief Const access operator
				/// @param index Column name
				/// @return Column value
				/// @exception std::out_of_range Wrong index
				const std::unordered_map<std::string, std::string>& operator [] (size_t index) const;

				/// @brief Access to first row
				/// @return First row
				/// @exception std::out_of_range Empty SQLiteResult
				std::unordered_map<std::string, std::string>& front();

				/// @brief Const access to first row
				/// @return First row
				/// @exception std::out_of_range Empty SQLiteResult
				const std::unordered_map<std::string, std::string>& front() const;

				/// @brief Access to last row
				/// @return Last row
				/// @exception std::out_of_range Empty SQLiteResult
				std::unordered_map<std::string, std::string>& back();

				/// @brief Const access to last row
				/// @return Last row
				/// @exception std::out_of_range Empty SQLiteResult
				const std::unordered_map<std::string, std::string>& back() const;

				/// @brief Access to row
				/// @param index Row index
				/// @return Row
				/// @exception std::out_of_range Wrong index
				std::unordered_map<std::string, std::string>& at(size_t index);
				
				/// @brief Const access to row
				/// @param index Row index
				/// @return Row
				/// @exception std::out_of_range Wrong index
				const std::unordered_map<std::string, std::string>& at(size_t index) const;

				/// @brief Access to column in row
				/// @param index Row index
				/// @param columnName Column name
				/// @return Value in column
				/// @exception std::out_of_range Wrong index
				std::string& at(size_t index, const std::string& columnName);

				/// @brief Const access to column in row
				/// @param index Row index
				/// @param columnName Column name
				/// @return Value in column
				/// @exception std::out_of_range Wrong index
				const std::string& at(size_t index, const std::string& columnName) const;

				/// @brief Begin iterator
				/// @return Iterator to first row
				const_iterator begin() const noexcept;

				/// @brief End iterator
				/// @return Iterator to row after last
				const_iterator end() const noexcept;

				~SQLiteResult() = default;
			};
		}
	}
}
