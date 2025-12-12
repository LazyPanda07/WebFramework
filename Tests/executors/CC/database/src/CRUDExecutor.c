#include <executors/executor.h>

#include <time.h>
#include <signal.h>

#include "SQLUtility.h"

DEFINE_DEFAULT_EXECUTOR(CRUDExecutor, STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(CRUDExecutor, GET_METHOD, request, response)
{
	database_t database;
	table_t table;
	sql_value_t value;
	sql_result_t result;
	json_builder_t builder;
	json_object_t data;

	wf_get_database_request(request, "test_database", &database);
	wf_get_table(database, "test_table", &table);
	wf_create_sql_value(&value);
	wf_create_json_builder(&builder);

	wf_set_sql_value_string(value, "glue");

	wf_execute_query(table, "SELECT * FROM test_table WHERE name = ?", &value, 1, &result);
	wf_iterate_sql_result(result, initBuffer, callback, &data);

	wf_append_json_builder_object(builder, "data", &data);

	wf_set_json_body(response, builder);

	wf_delete_json_object(&data);
	wf_delete_web_framework_sql_value(value);
	wf_delete_web_framework_json_builder(builder);
	wf_delete_sql_result(table, result);
}

DEFINE_EXECUTOR_METHOD(CRUDExecutor, POST_METHOD, request, response)
{
	database_t database;
	table_t table;

	wf_get_or_create_database_request(request, "test_database", &database);
	wf_get_or_create_table
	(
		database,
		"test_table",
		"CREATE TABLE IF NOT EXISTS test_table ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name VARCHAR(255) NOT NULL, "
		"amount INTEGER NOT NULL)",
		&table
	);
}

DEFINE_EXECUTOR_METHOD(CRUDExecutor, PUT_METHOD, request, response)
{
	database_t database;
	table_t table;
	sql_result_t result;
	sql_value_t* values = (sql_value_t*)malloc(2 * sizeof(sql_value_t));

	if (!values)
	{
		raise(SIGSEGV);

		return;
	}

	srand(time(NULL));

	wf_get_database_request(request, "test_database", &database);
	wf_get_table(database, "test_table", &table);

	wf_create_sql_value(&(values[0]));
	wf_create_sql_value(&(values[1]));

	wf_set_sql_value_string(values[0], "glue");
	wf_set_sql_value_int(values[1], -1);

	wf_execute_query(table, "INSERT INTO test_table (name, amount) VALUES(?, ?)", values, 2, &result);

	for (size_t i = 0; i < 10; i++)
	{
		wf_set_sql_value_int(values[1], rand() % 200);

		wf_execute_query(table, "INSERT INTO test_table (name, amount) VALUES(?, ?)", values, 2, &result);
	}

	free(values);
}

DEFINE_EXECUTOR_METHOD(CRUDExecutor, PATCH_METHOD, request, response)
{
	database_t database;
	table_t table;
	sql_result_t result;
	sql_value_t* values = (sql_value_t*)malloc(2 * sizeof(sql_value_t));
	json_builder_t builder;
	json_object_t data;

	if (!values)
	{
		raise(SIGSEGV);

		return;
	}

	wf_get_database_request(request, "test_database", &database);
	wf_get_table(database, "test_table", &table);
	wf_create_json_builder(&builder);

	wf_create_sql_value(&(values[0]));
	wf_create_sql_value(&(values[1]));

	wf_set_sql_value_string(values[0], "empty");
	wf_set_sql_value_int(values[1], -1);

	wf_execute_query
	(
		table,
		"UPDATE test_table "
		"SET name = ? "
		"WHERE amount = ?",
		values,
		2,
		&result
	);

	wf_execute_query
	(
		table,
		"SELECT * FROM test_table WHERE name = ?",
		values,
		1,
		&result
	);

	wf_iterate_sql_result(result, initBuffer, callback, &data);

	wf_append_json_builder_object(builder, "data", &data);

	wf_set_json_body(response, builder);

	wf_delete_json_object(&data);
	wf_delete_web_framework_sql_value(values[0]);
	wf_delete_web_framework_sql_value(values[1]);
	wf_delete_web_framework_json_builder(builder);
}

DEFINE_INITIALIZE_WEB_FRAMEWORK();
