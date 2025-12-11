#include "SQLUtility.h"

#include <JsonParser.h>

void initBuffer(size_t size, void* buffer)
{
	JsonObject_t* data = (JsonObject_t*)buffer;

	createJsonObject(data);
}

void callback(const char** columnNames, const SQLValue* columnValues, size_t size, size_t index, void* buffer)
{
	JsonObject_t* array = (JsonObject_t*)buffer;
	JsonObject_t object;

	emplaceJsonObject(array, &object);

	for (size_t i = 0; i < size; i++)
	{
		SQLValueType_t type;
		JsonObject_t temp;

		getSQLValueType(columnValues[i], &type);

		if (type == INT_TYPE)
		{
			int64_t result;

			getSQLValueInt(columnValues[i], &result);

			assignJsonObject(&object, columnNames[i], &temp);

			setJsonObjectInteger(&temp, result);
		}
		else if (type == STRING_TYPE)
		{
			const char* result;

			getSQLValueString(columnValues[i], &result);

			assignJsonObject(&object, columnNames[i], &temp);

			setJsonObjectString(&temp, result);
		}
	}
}
