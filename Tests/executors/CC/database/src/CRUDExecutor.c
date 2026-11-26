#include <executors/executor.h>

#include <time.h>
#include <signal.h>

#include "SQLUtility.h"

DEFINE_DEFAULT_EXECUTOR(CRUDExecutor, STATELESS_EXECUTOR)

DEFINE_EXECUTOR_INIT(CRUDExecutor)
{
#ifndef WITHOUT_POSTGRES_TESTS
	{
		database_t database;
		table_t table;
		const char* host = "127.0.0.1";
		const char* password = "root";
		char* overrideHost = getenv("POSTGRES_HOST");
		char* overridePassword = getenv("POSTGRES_PASSWORD");

		if (overrideHost)
		{
			host = overrideHost;
		}

		if (overridePassword)
		{
			password = overridePassword;
		}

		wf_construct_database_executor_settings(settings, "test_database", printf("host=%s port=5432 dbname=test_database user=postgres password=%s", host, password), POSTGRES_DATABASE_IMPLEMENTATION_NAME, &database);
		wf_get_or_create_table
		(
			database,
			"test_table",
			"CREATE TABLE IF NOT EXISTS test_table ("
			"id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY, "
			"name VARCHAR(255) NOT NULL, "
			"amount INTEGER NOT NULL)",
			&table
		);
	}
#endif

	{
		database_t database;
		table_t table;

		wf_get_or_create_database_executor_settings(settings, "test_database", SQLITE_DATABASE_IMPLEMENTATION_NAME, &database);
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
}

DEFINE_EXECUTOR_METHOD(CRUDExecutor, GET_METHOD, request, response)
{
	database_t database;
	table_t table;
	sql_value_t value;
	sql_result_t result;
	json_builder_t builder;
	json_object_t data;
	char* database_implementation = NULL;

	wf_get_route_string_parameter(request, "databaseImplementation", &database_implementation);

	wf_get_database_request(request, "test_database", database_implementation, &database);
	wf_get_table(database, "test_table", &table);
	wf_create_sql_value(&value);
	wf_create_json_builder(&builder);

	wf_set_sql_value_string(value, "glue");

	if (!strcmp(database_implementation, SQLITE_DATABASE_IMPLEMENTATION_NAME))
	{
		wf_execute_query(table, "SELECT * FROM test_table WHERE name = ?", &value, 1, &result);
	}
	else if (!strcmp(database_implementation, POSTGRES_DATABASE_IMPLEMENTATION_NAME))
	{
		wf_execute_query(table, "SELECT * FROM test_table WHERE name = $1", &value, 1, &result);
	}

	wf_iterate_sql_result(result, initBuffer, callback, &data);

	wf_append_json_builder_object(builder, "data", &data);

	wf_set_json_body(response, builder);

	wf_delete_json_object(&data);
	wf_delete_sql_value(value);
	wf_delete_json_builder(builder);
	wf_delete_sql_result(table, result);
}

DEFINE_EXECUTOR_METHOD(CRUDExecutor, PUT_METHOD, request, response)
{
	database_t database;
	table_t table;
	sql_result_t result;
	sql_value_t* values = (sql_value_t*)malloc(2 * sizeof(sql_value_t));
	char* database_implementation = NULL;

	wf_get_route_string_parameter(request, "databaseImplementation", &database_implementation);

	if (!values)
	{
		raise(SIGSEGV);

		return;
	}

	srand(time(NULL));

	wf_get_database_request(request, "test_database", database_implementation, &database);
	wf_get_table(database, "test_table", &table);

	wf_create_sql_value(&(values[0]));
	wf_create_sql_value(&(values[1]));

	wf_set_sql_value_string(values[0], "glue");
	wf_set_sql_value_int(values[1], -1);

	if (!strcmp(database_implementation, SQLITE_DATABASE_IMPLEMENTATION_NAME))
	{
		wf_execute_query(table, "INSERT INTO test_table (name, amount) VALUES(?, ?)", values, 2, &result);
	}
	else if (!strcmp(database_implementation, POSTGRES_DATABASE_IMPLEMENTATION_NAME))
	{
		wf_execute_query(table, "INSERT INTO test_table (name, amount) VALUES($1, $2)", values, 2, &result);
	}

	for (size_t i = 0; i < 10; i++)
	{
		wf_set_sql_value_int(values[1], rand() % 200);

		if (!strcmp(database_implementation, SQLITE_DATABASE_IMPLEMENTATION_NAME))
		{
			wf_execute_query(table, "INSERT INTO test_table (name, amount) VALUES(?, ?)", values, 2, &result);
		}
		else if (!strcmp(database_implementation, POSTGRES_DATABASE_IMPLEMENTATION_NAME))
		{
			wf_execute_query(table, "INSERT INTO test_table (name, amount) VALUES($1, $2)", values, 2, &result);
		}
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
	char* database_implementation = NULL;

	wf_get_route_string_parameter(request, "databaseImplementation", &database_implementation);

	if (!values)
	{
		raise(SIGSEGV);

		return;
	}

	wf_get_database_request(request, "test_database", database_implementation, &database);
	wf_get_table(database, "test_table", &table);
	wf_create_json_builder(&builder);

	wf_create_sql_value(&(values[0]));
	wf_create_sql_value(&(values[1]));

	wf_set_sql_value_string(values[0], "empty");
	wf_set_sql_value_int(values[1], -1);

	if (!strcmp(database_implementation, SQLITE_DATABASE_IMPLEMENTATION_NAME))
	{
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
	}
	else if (!strcmp(database_implementation, POSTGRES_DATABASE_IMPLEMENTATION_NAME))
	{
		wf_execute_query
		(
			table,
			"UPDATE test_table "
			"SET name = $1 "
			"WHERE amount = $2",
			values,
			2,
			&result
		);

		wf_execute_query
		(
			table,
			"SELECT * FROM test_table WHERE name = $1",
			values,
			1,
			&result
		);
	}

	wf_iterate_sql_result(result, initBuffer, callback, &data);

	wf_append_json_builder_object(builder, "data", &data);

	wf_set_json_body(response, builder);

	wf_delete_json_object(&data);
	wf_delete_sql_value(values[0]);
	wf_delete_sql_value(values[1]);
	wf_delete_json_builder(builder);
}

DEFINE_INITIALIZE_WEB_FRAMEWORK()
