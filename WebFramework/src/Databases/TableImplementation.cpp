#include "TableImplementation.h"

#include <stdexcept>

#include "SQLResultImplementation.h"

using namespace std;

namespace framework
{
	TableImplementation::TableImplementation(database::Table* table) :
		table(table)
	{

	}

	interfaces::ISQLResult* TableImplementation::execute(const char* query, const interfaces::ISQLValue** values, size_t size)
	{
		vector<database::SQLValue> temp;

		temp.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			const interfaces::ISQLValue& value = *values[i];

			switch (value.getType())
			{
			case interfaces::ISQLValue::SQLValueType::intType:
				temp.emplace_back(value.getInt());

				break;

			case interfaces::ISQLValue::SQLValueType::stringType:
				temp.emplace_back(value.getString());

				break;

			case interfaces::ISQLValue::SQLValueType::doubleType:
				temp.emplace_back(value.getDouble());

				break;

			case interfaces::ISQLValue::SQLValueType::boolType:
				temp.emplace_back(value.getBool());

				break;

			case interfaces::ISQLValue::SQLValueType::nullptrType:
				temp.emplace_back(nullptr);

				break;

			case interfaces::ISQLValue::SQLValueType::blobType:
			{
				size_t blobSize = 0;
				const uint8_t* blobData = value.getBlob(&blobSize);

				temp.emplace_back(vector<uint8_t>{ blobData, blobData + blobSize });
			}

			break;

			default:
				throw runtime_error("Wrong type");
			}
		}

		return new SQLResultImplementation(table->execute<database::RawQuery>(temp, query));
	}

	void TableImplementation::deleteResult(interfaces::ISQLResult* ptr) const
	{
		delete ptr;
	}

	const char* TableImplementation::getTableName() const
	{
		return table->getTableName().data();
	}
}
