#include <executors/executor.h>

#include <signal.h>

#include <utility/web_framework_utility.h>

#include "SQLUtility.h"

typedef struct
{
	web_framework_string_t uuid;
} MultiUserExecutor;

DEFINE_EXECUTOR(MultiUserExecutor, HEAVY_OPERATION_STATEFUL_EXECUTOR);

DEFINE_EXECUTOR_INIT(MultiUserExecutor)
{
	MultiUserExecutor* self = (MultiUserExecutor*)executor;

	wf_generate_uuid(&self->uuid);
}

DEFINE_EXECUTOR_METHOD(MultiUserExecutor, GET_METHOD, request, response)
{
	MultiUserExecutor* self = (MultiUserExecutor*)executor;
	table_t table;
	sql_value_t value;
	sql_result_t result;
	json_builder_t builder;
	json_object_t data;
	
	wf_get_table_request(request, "test_database", "multi_user", &table);
	wf_create_sql_value(&value);
	wf_create_json_builder(&builder);

	wf_set_sql_value_string(value, wf_get_data_from_string(self->uuid));

	wf_execute_query(table, "SELECT * FROM multi_user WHERE user_id = ?", &value, 1, &result);

	wf_iterate_sql_result(result, initBuffer, callback, &data);

	wf_append_json_builder_object(builder, "data", &data);

	wf_set_json_body(response, builder);

	wf_delete_json_object(&data);
	wf_delete_value(value);
	wf_delete_json_builder(builder);
	wf_delete_sql_result(table, result);
}

DEFINE_EXECUTOR_METHOD(MultiUserExecutor, POST_METHOD, request, response)
{
	database_t database;
	table_t table;

	wf_get_or_create_database_request(request, "test_database", &database);
	wf_get_or_create_table(database, "multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)", &table);
}

DEFINE_EXECUTOR_METHOD(MultiUserExecutor, PUT_METHOD, request, response)
{
	MultiUserExecutor* self = (MultiUserExecutor*)executor;
	table_t table;
	sql_value_t* values = (sql_value_t*)malloc(2 * sizeof(sql_value_t));
	sql_result_t result;
	json_parser_t parser;
	const char* data = NULL;

	if (!values)
	{
		raise(SIGSEGV);

		return;
	}

	wf_get_table_request(request, "test_database", "multi_user", &table);
	wf_create_sql_value(&values[0]);
	wf_create_sql_value(&values[1]);
	wf_get_request_json(request, &parser);

	wf_get_json_parser_string(parser, "data", true, &data);
	wf_set_sql_value_string(values[0], wf_get_data_from_string(self->uuid));
	wf_set_sql_value_string(values[1], data);

	wf_execute_query(table, "INSERT INTO multi_user (user_id, data) VALUES(?, ?)", values, 2, &result);

	wf_delete_sql_value(values[0]);
	wf_delete_sql_value(values[1]);
	wf_delete_sql_result(table, result);

	free(values);
}

DEFINE_EXECUTOR_DESTROY(MultiUserExecutor)
{
	MultiUserExecutor* self = (MultiUserExecutor*)executor;

	wf_delete_string(self->uuid);
}
