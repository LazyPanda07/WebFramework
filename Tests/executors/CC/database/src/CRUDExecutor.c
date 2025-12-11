#include <Executors/Executor.h>

#include <time.h>
#include <signal.h>

#include "SQLUtility.h"

DEFINE_DEFAULT_EXECUTOR(CRUDExecutor, STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(CRUDExecutor, GET_METHOD, request, response)
{
	Database database;
	Table table;
	SQLValue value;
	SQLResult result;
	JsonBuilder builder;
	JsonObject_t data;

	getDatabaseHTTPRequest(request, "test_database", &database);
	getTable(database, "test_table", &table);
	createSQLValue(&value);
	createJsonBuilder(&builder);

	setSQLValueString(value, "glue");

	executeQuery(table, "SELECT * FROM test_table WHERE name = ?", &value, 1, &result);
	iterateSQLResult(result, initBuffer, callback, &data);

	appendJsonBuilderObject(builder, "data", &data);

	setJsonBody(response, builder);

	deleteJsonObject(&data);
	deleteWebFrameworkSQLValue(value);
	deleteWebFrameworkJsonBuilder(builder);
	deleteSQLResult(table, result);
}

DEFINE_EXECUTOR_METHOD(CRUDExecutor, POST_METHOD, request, response)
{
	Database database;
	Table table;

	getOrCreateDatabaseHTTPRequest(request, "test_database", &database);
	getOrCreateTable
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
	Database database;
	Table table;
	SQLResult result;
	SQLValue* values = (SQLValue*)malloc(2 * sizeof(SQLValue));

	if (!values)
	{
		raise(SIGSEGV);

		return;
	}

	srand(time(NULL));

	getDatabaseHTTPRequest(request, "test_database", &database);
	getTable(database, "test_table", &table);

	createSQLValue(&(values[0]));
	createSQLValue(&(values[1]));

	setSQLValueString(values[0], "glue");
	setSQLValueInt(values[1], -1);

	executeQuery(table, "INSERT INTO test_table (name, amount) VALUES(?, ?)", values, 2, &result);

	for (size_t i = 0; i < 10; i++)
	{
		setSQLValueInt(values[1], rand() % 200);

		executeQuery(table, "INSERT INTO test_table (name, amount) VALUES(?, ?)", values, 2, &result);
	}

	free(values);
}

DEFINE_EXECUTOR_METHOD(CRUDExecutor, PATCH_METHOD, request, response)
{
	Database database;
	Table table;
	SQLResult result;
	SQLValue* values = (SQLValue*)malloc(2 * sizeof(SQLValue));
	JsonBuilder builder;
	JsonObject_t data;

	if (!values)
	{
		raise(SIGSEGV);

		return;
	}

	getDatabaseHTTPRequest(request, "test_database", &database);
	getTable(database, "test_table", &table);
	createJsonBuilder(&builder);

	createSQLValue(&(values[0]));
	createSQLValue(&(values[1]));

	setSQLValueString(values[0], "empty");
	setSQLValueInt(values[1], -1);

	executeQuery
	(
		table,
		"UPDATE test_table "
		"SET name = ? "
		"WHERE amount = ?",
		values,
		2,
		&result
	);

	executeQuery
	(
		table,
		"SELECT * FROM test_table WHERE name = ?",
		values,
		1,
		&result
	);

	iterateSQLResult(result, initBuffer, callback, &data);

	appendJsonBuilderObject(builder, "data", &data);

	setJsonBody(response, builder);

	deleteJsonObject(&data);
	deleteWebFrameworkSQLValue(values[0]);
	deleteWebFrameworkSQLValue(values[1]);
	deleteWebFrameworkJsonBuilder(builder);
}

DEFINE_INITIALIZE_WEB_FRAMEWORK();
