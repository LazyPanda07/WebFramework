#include "SQLUtility.h"

#include <JsonParser.h>

void initBuffer(size_t size, void* buffer)
{
	JSONArray_t* array = (JSONArray_t*)buffer;

	*array = createJSONArray(size);
}

void callback(const char** columnNames, const SQLValue* columnValues, size_t size, size_t index, void* buffer)
{
	JSONArray_t* array = (JSONArray_t*)buffer;
	JSONObject_t object;

	createJSONObject(&object);

	for (size_t i = 0; i < size; i++)
	{
		SQLValueType_t type;
		JSONObject_t temp;

		getSQLValueType(columnValues[i], &type);

		if (type == INT_TYPE)
		{
			int64_t result;

			getSQLValueInt(columnValues[i], &result);

			assignJSONObject(&object, columnNames[i], &temp);

			setJSONObjectInteger(&temp, result);
		}
		else if (type == STRING_TYPE)
		{
			const char* result;

			getSQLValueString(columnValues[i], &result);

			assignJSONObject(&object, columnNames[i], &temp);

			setJSONObjectString(&temp, result);
		}
	}

	appendJSONArrayObject(array, &object);
	deleteJSONObject(&object);
}
