#include "SQLValueImplementation.h"

namespace framework
{
	SQLValueImplementation::SQLValueImplementation(const database::SQLValue& value) :
		value(value)
	{

	}

	SQLValueImplementation::SQLValueImplementation(database::SQLValue&& value) :
		value(std::move(value))
	{

	}

	void SQLValueImplementation::setInt(int64_t value)
	{
		this->value = value;
	}

	void SQLValueImplementation::setDouble(double value)
	{
		this->value = value;
	}

	void SQLValueImplementation::setString(const char* value)
	{
		this->value = value;
	}

	void SQLValueImplementation::setBool(bool value)
	{
		this->value = value;
	}

	void SQLValueImplementation::setNull()
	{
		this->value = nullptr;
	}

	void SQLValueImplementation::setBlob(const uint8_t* value, size_t size)
	{
		this->value = std::vector<uint8_t>(value, value + size);
	}

	int64_t SQLValueImplementation::getInt() const
	{
		return value.get<int64_t>();
	}

	double SQLValueImplementation::getDouble() const
	{
		return value.get<double>();
	}

	const char* SQLValueImplementation::getString() const
	{
		return value.get<std::string>().data();
	}

	bool SQLValueImplementation::getBool() const
	{
		return value.get<bool>();
	}

	const uint8_t* SQLValueImplementation::getBlob(size_t* size) const
	{
		const std::vector<uint8_t>& result = value.get<std::vector<uint8_t>>();

		*size = result.size();

		return result.data();
	}

	SQLValueImplementation::SQLValueType SQLValueImplementation::getType() const
	{
		return static_cast<SQLValueType>((*value).index());
	}
}
