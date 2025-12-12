#include "SQLUtility.h"

#include <json_parser.h>

void initBuffer(size_t size, void* buffer)
{
	json_object_t* data = (json_object_t*)buffer;

	wf_create_json_object(data);
}

void callback(const char** columnNames, const sql_value_t* columnValues, size_t size, size_t index, void* buffer)
{
	json_object_t* array = (json_object_t*)buffer;
	json_object_t object;

	wf_emplace_json_object(array, &object);

	for (size_t i = 0; i < size; i++)
	{
		sql_value_type_t type;
		json_object_t temp;

		wf_get_sql_value_type(columnValues[i], &type);

		if (type == INT_TYPE)
		{
			int64_t result;

			wf_get_sql_value_int(columnValues[i], &result);

			wf_assign_json_object(&object, columnNames[i], &temp);

			wf_set_json_object_integer(&temp, result);
		}
		else if (type == STRING_TYPE)
		{
			const char* result;

			wf_get_sql_value_string(columnValues[i], &result);

			wf_assign_json_object(&object, columnNames[i], &temp);

			wf_set_json_object_string(&temp, result);
		}
	}
}
