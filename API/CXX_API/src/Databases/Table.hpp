#pragma once

#include "DatabaseInterfaces/ITable.h"

#include "SQLResult.hpp"

namespace framework
{
	class Table
	{
	private:
		interfaces::ITable* implementation;

	public:
		Table(interfaces::ITable* implementation);

		SQLResult execute(std::string_view query, const std::vector<SQLValue>& values = {});

		~Table() = default;
	};
}

namespace __framework
{
	class CSQLValue : public framework::interfaces::ISQLValue
	{
	private:
		const framework::SQLValue& value;

	public:
		CSQLValue(const framework::SQLValue& value);

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

	inline CSQLValue::CSQLValue(const framework::SQLValue& value) :
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

	inline SQLResult Table::execute(std::string_view query, const std::vector<SQLValue>& values)
	{
		std::vector<__framework::CSQLValue> tempValues;
		std::vector<const interfaces::ISQLValue*> pointers;

		tempValues.reserve(values.size());
		pointers.reserve(values.size());

		for (const SQLValue& value : values)
		{
			pointers.push_back(&tempValues.emplace_back(value));
		}

		interfaces::ISQLResult* tempResult = implementation->execute(query.data(), pointers.data(), pointers.size());
		SQLResult result(tempResult);

		implementation->deleteResult(tempResult);

		return result;
	}
}
