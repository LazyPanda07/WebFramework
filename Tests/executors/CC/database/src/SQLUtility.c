#include "SQLUtility.h"

#include <JSONParser.h>

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

		getSQLValueType(columnValues[i], &type);

		if (type == INT_TYPE)
		{
			int64_t result;

			getSQLValueInt(columnValues[i], &result);

			setJSONObjectInteger(&object, columnNames[i], result);
		}
		else if (type == STRING_TYPE)
		{
			const char* result;

			getSQLValueString(columnValues[i], &result);

			setJSONObjectString(&object, columnNames[i], result);
		}
	}

	appendJSONArrayObject(array, &object);
	deleteJSONObject(&object);
}
