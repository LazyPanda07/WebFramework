#include "TableExecutors.h"

class CSQLValue : public framework::interfaces::ISQLValue
{
private:
	const framework::SQLValueExecutors& value;

public:
	CSQLValue(const framework::SQLValueExecutors& value);

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

namespace framework
{
	TableExecutors::TableExecutors(interfaces::ITable* implementation) :
		implementation(implementation)
	{

	}

	SQLResultExecutors TableExecutors::execute(std::string_view query, const std::vector<SQLValueExecutors>& values)
	{
		std::vector<CSQLValue> tempValues;
		std::vector<const interfaces::ISQLValue*> pointers;

		tempValues.reserve(values.size());
		pointers.reserve(values.size());

		for (const SQLValueExecutors& value : values)
		{
			pointers.push_back(&tempValues.emplace_back(value));
		}

		interfaces::ISQLResult* tempResult = implementation->execute(query.data(), pointers.data(), pointers.size());
		SQLResultExecutors result(tempResult);

		implementation->deleteResult(tempResult);

		return result;
	}
}

CSQLValue::CSQLValue(const framework::SQLValueExecutors& value) :
	value(value)
{

}

void CSQLValue::setInt(int64_t value)
{

}

void CSQLValue::setDouble(double value)
{

}

void CSQLValue::setString(const char* value)
{

}

void CSQLValue::setBool(bool value)
{

}

void CSQLValue::setNull()
{

}

void CSQLValue::setBlob(const uint8_t* value, size_t size)
{

}

int64_t CSQLValue::getInt() const
{
	return value.get<int64_t>();
}

double CSQLValue::getDouble() const
{
	return value.get<double>();
}

const char* CSQLValue::getString() const
{
	return value.get<std::string>().data();
}

bool CSQLValue::getBool() const
{
	return value.get<bool>();
}

const uint8_t* CSQLValue::getBlob(size_t* size) const
{
	const std::vector<uint8_t>& data = value.get<std::vector<uint8_t>>();

	*size = data.size();

	return data.data();
}

CSQLValue::SQLValueType CSQLValue::getType() const
{
	return static_cast<SQLValueType>((*value).index());
}
