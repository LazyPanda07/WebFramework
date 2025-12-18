#pragma once

#include <array>

#include "DatabaseInterfaces/ITable.hpp"

#include "SqlResult.hpp"

namespace framework
{
	class Table
	{
	private:
		interfaces::ITable* implementation;

	private:
		Table(interfaces::ITable* implementation);

	public:
		/**
		 * @brief Executes the specified SQL query with optional parameter values and returns the execution result.
		 * @param query The SQL query or statement to execute.
		 * @param values Optional vector of values to bind to parameter placeholders in the query (defaults to empty).
		 * @return SqlResult representing the outcome of the execution (e.g., returned rows, affected row count, and status).
		 */
		SqlResult execute(std::string_view query, const std::vector<SqlValue>& values = {});

		/**
		 * @brief Executes an SQL query using a fixed-size array of parameter values.
		 * @tparam SizeT The compile-time number of elements in the values array (the number of parameters).
		 * @param query The SQL statement to execute, provided as a std::string_view.
		 * @param values A const reference to a std::array of SqlValue items to bind to the query parameters.
		 * @return An SqlResult representing the outcome of the execution (e.g., status, affected rows, or returned result set).
		 */
		template<size_t SizeT>
		SqlResult execute(std::string_view query, const std::array<SqlValue, SizeT>& values);

		~Table() = default;

		friend class Database;
	};
}

namespace __framework
{
	class CSQLValue : public framework::interfaces::ISQLValue
	{
	private:
		const framework::SqlValue& value;

	public:
		CSQLValue(const framework::SqlValue& value);

		void setInt(int64_t value) override;

		void setDouble(double value) override;

		void setString(const char* value) override;

		void setBool(bool value) override;

		void setNull() override;

		void setBlob(const uint8_t* value, size_t size) override;

		int64_t getInt() const override;

		double getDouble() const override;

		const char* getString() const override;

		bool getBool() const override;

		const uint8_t* getBlob(size_t* size) const override;

		SQLValueType getType() const override;

		~CSQLValue() = default;
	};

	inline CSQLValue::CSQLValue(const framework::SqlValue& value) :
		value(value)
	{

	}

	inline void CSQLValue::setInt(int64_t value)
	{

	}

	inline void CSQLValue::setDouble(double value)
	{

	}

	inline void CSQLValue::setString(const char* value)
	{

	}

	inline void CSQLValue::setBool(bool value)
	{

	}

	inline void CSQLValue::setNull()
	{

	}

	inline void CSQLValue::setBlob(const uint8_t* value, size_t size)
	{

	}

	inline int64_t CSQLValue::getInt() const
	{
		return value.get<int64_t>();
	}

	inline double CSQLValue::getDouble() const
	{
		return value.get<double>();
	}

	inline const char* CSQLValue::getString() const
	{
		return value.get<std::string>().data();
	}

	inline bool CSQLValue::getBool() const
	{
		return value.get<bool>();
	}

	inline const uint8_t* CSQLValue::getBlob(size_t* size) const
	{
		const std::vector<uint8_t>& data = value.get<std::vector<uint8_t>>();

		*size = data.size();

		return data.data();
	}

	inline CSQLValue::SQLValueType CSQLValue::getType() const
	{
		return static_cast<SQLValueType>((*value).index());
	}
}

namespace framework
{
	inline Table::Table(interfaces::ITable* implementation) :
		implementation(implementation)
	{

	}

	inline SqlResult Table::execute(std::string_view query, const std::vector<SqlValue>& values)
	{
		std::vector<__framework::CSQLValue> tempValues;
		std::vector<const interfaces::ISQLValue*> pointers;

		tempValues.reserve(values.size());
		pointers.reserve(values.size());

		for (const SqlValue& value : values)
		{
			pointers.push_back(&tempValues.emplace_back(value));
		}

		interfaces::ISQLResult* tempResult = implementation->execute(query.data(), pointers.data(), pointers.size());
		SqlResult result(tempResult);

		implementation->deleteResult(tempResult);

		return result;
	}

	template<size_t SizeT>
	inline SqlResult Table::execute(std::string_view query, const std::array<SqlValue, SizeT>& values)
	{
		std::vector<__framework::CSQLValue> tempValues;
		std::vector<const interfaces::ISQLValue*> pointers;

		tempValues.reserve(SizeT);
		pointers.reserve(SizeT);

		for (const SqlValue& value : values)
		{
			pointers.push_back(&tempValues.emplace_back(value));
		}

		interfaces::ISQLResult* tempResult = implementation->execute(query.data(), pointers.data(), pointers.size());
		SqlResult result(tempResult);

		implementation->deleteResult(tempResult);

		return result;
	}
}
