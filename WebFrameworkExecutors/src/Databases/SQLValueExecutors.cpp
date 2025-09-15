#include "SQLValueExecutors.h"

#include <stdexcept>

namespace framework
{
	SQLValueExecutors::SQLValueExecutors(const interfaces::ISQLValue* implementation)
	{
		switch (implementation->getType())
		{
		case interfaces::ISQLValue::SQLValueType::intType:
			value = implementation->getInt();

			break;

		case interfaces::ISQLValue::SQLValueType::doubleType:
			value = implementation->getDouble();

			break;

		case interfaces::ISQLValue::SQLValueType::stringType:
			value = implementation->getString();

			break;

		case interfaces::ISQLValue::SQLValueType::boolType:
			value = implementation->getBool();

			break;

		case interfaces::ISQLValue::SQLValueType::nullptrType:
			value = nullptr;

			break;

		case interfaces::ISQLValue::SQLValueType::blobType:
		{
			size_t size = 0;
			const uint8_t* data = implementation->getBlob(&size);

			value = std::vector<uint8_t>(data, data + size);
		}

		break;

		default:
			throw std::runtime_error("Wrong type from framework::interfaces::ISQLValue::SQLValueType");
		}
	}

	SQLValueExecutors::SQLValueExecutors(const ValueType& value) :
		value(value)
	{

	}

	SQLValueExecutors::SQLValueExecutors(ValueType&& value) noexcept :
		value(move(value))
	{

	}

	const SQLValueExecutors::ValueType& SQLValueExecutors::operator *() const
	{
		return value;
	}
}
