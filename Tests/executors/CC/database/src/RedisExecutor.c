#include <executors/executor.h>

#include "SQLUtility.h"

static void redisInitBuffer(size_t size, void* buffer);

static void redisCallback(const char** columnNames, const sql_value_t* columnValues, size_t size, size_t index, void* buffer);

DEFINE_DEFAULT_EXECUTOR(RedisExecutor, STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(RedisExecutor, GET_METHOD, request, response)
{
	table_t connect;
	sql_value_t key;
	sql_result_t temp;
	json_builder_t builder;
	
	wf_get_table_request(request, "127.0.0.1:10010:password", REDIS_DATABASE_IMPLEMENTATION_NAME, "", &connect);
	wf_create_sql_value(&key);
	wf_create_json_builder(&builder);

	wf_set_sql_value_string(key, "string");
	wf_execute_query(connect, "GET", &key, 1, &temp);
	wf_iterate_sql_result(temp, redisInitBuffer, redisCallback, builder);
	wf_delete_sql_result(connect, temp);

	wf_set_sql_value_string(key, "int");
	wf_execute_query(connect, "GET", &key, 1, &temp);
	wf_iterate_sql_result(temp, redisInitBuffer, redisCallback, builder);
	wf_delete_sql_result(connect, temp);

	wf_set_sql_value_string(key, "double");
	wf_execute_query(connect, "GET", &key, 1, &temp);
	wf_iterate_sql_result(temp, redisInitBuffer, redisCallback, builder);
	wf_delete_sql_result(connect, temp);

	wf_set_sql_value_string(key, "bool");
	wf_execute_query(connect, "GET", &key, 1, &temp);
	wf_iterate_sql_result(temp, redisInitBuffer, redisCallback, builder);
	wf_delete_sql_result(connect, temp);

	wf_set_json_body(response, builder);

	wf_delete_sql_value(key);
	wf_delete_json_builder(builder);
}

DEFINE_EXECUTOR_METHOD(RedisExecutor, POST_METHOD, request, response)
{
	database_t database;
	table_t connect;

	wf_get_or_create_database_request(request, "127.0.0.1:10010:password", REDIS_DATABASE_IMPLEMENTATION_NAME, &database);

	wf_get_or_create_table(database, "", "", &connect);

	wf_set_http_response_code(response, CREATED);
}

DEFINE_EXECUTOR_METHOD(RedisExecutor, PUT_METHOD, request, response)
{
	table_t connect;
	sql_value_t keyValue[2];
	sql_result_t temp;

	wf_get_table_request(request, "127.0.0.1:10010:password", REDIS_DATABASE_IMPLEMENTATION_NAME, "", &connect);
	wf_create_sql_value(&keyValue[0]);
	wf_create_sql_value(&keyValue[1]);
	
	wf_set_sql_value_string(keyValue[0], "string");
	wf_set_sql_value_string(keyValue[1], "qwe");

	wf_execute_query(connect, "SET", keyValue, sizeof(keyValue) / sizeof(sql_value_t), &temp);
	wf_delete_sql_result(connect, temp);

	wf_set_sql_value_string(keyValue[0], "int");
	wf_set_sql_value_int(keyValue[1], 5);

	wf_execute_query(connect, "SET", keyValue, sizeof(keyValue) / sizeof(sql_value_t), &temp);
	wf_delete_sql_result(connect, temp);

	wf_set_sql_value_string(keyValue[0], "double");
	wf_set_sql_value_double(keyValue[1], 2.3);

	wf_execute_query(connect, "SET", keyValue, sizeof(keyValue) / sizeof(sql_value_t), &temp);
	wf_delete_sql_result(connect, temp);

	wf_set_sql_value_string(keyValue[0], "bool");
	wf_set_sql_value_bool(keyValue[1], true);

	wf_execute_query(connect, "SET", keyValue, sizeof(keyValue) / sizeof(sql_value_t), &temp);
	wf_delete_sql_result(connect, temp);

	wf_delete_sql_value(keyValue[0]);
	wf_delete_sql_value(keyValue[1]);

	wf_set_http_response_code(response, CREATED);
}

void redisInitBuffer(size_t size, void* buffer)
{
	
}

void redisCallback(const char** columnNames, const sql_value_t* columnValues, size_t size, size_t index, void* buffer)
{
	json_builder_t builder = (json_builder_t)buffer;
	sql_value_type_t type;

	wf_get_sql_value_type(columnValues[0], &type);

	if (type == STRING_TYPE)
	{
		const char* result;

		wf_get_sql_value_string(columnValues[0], &result);

		wf_append_json_builder_string(builder, "string", result);
	}
	else if (type == INT_TYPE)
	{
		int64_t result;

		wf_get_sql_value_int(columnValues[0], &result);
		
		if (result == 1)
		{
			wf_append_json_builder_boolean(builder, "bool", (bool)result);
		}
		else
		{
			wf_append_json_builder_integer(builder, "int", result);
		}
	}
	else if (type == DOUBLE_TYPE)
	{
		double result;

		wf_get_sql_value_double(columnValues[0], &result);

		wf_append_json_builder_double(builder, "double", result);
	}
}
