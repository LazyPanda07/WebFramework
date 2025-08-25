#include <APIExecutors/BaseExecutor.h>

#include <time.h>
#include <signal.h>

static void initBuffer(size_t size, void* buffer);

static void callback(const char* columnName, const SQLValue columnValue, size_t index, size_t size, void* buffer);

DECLARE_DEFAULT_EXECUTOR(CRUDExecutor, STATELESS_EXECUTOR);

DECLARE_EXECUTOR_METHOD(CRUDExecutor, GET_METHOD, request, response)
{
	Database database;
	Table table;
	SQLValue value;
	SQLResult result;
	JSONArray data;
	JSONBuilder builder;

	getDatabaseHTTPRequest(request, "test_database", &database);
	getTable(database, "test_table", &table);
	createSQLValue(&value);
	createJSONBuilder(&builder);

	setSQLValueString(value, "glue");

	executeQuery(table, "SELECT * FROM test_table WHERE name = ?", &value, 1, &result);
	iterateSQLResult(result, initBuffer, callback, &data);

	appendJSONBuilderArray(builder, "data", &data);
	setJSONBody(response, builder);

	deleteWebFrameworkSQLValue(value);
	deleteJSONArray(&data);
	deleteWebFrameworkJSONBuider(builder);
}

DECLARE_EXECUTOR_METHOD(CRUDExecutor, POST_METHOD, request, response)
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

DECLARE_EXECUTOR_METHOD(CRUDExecutor, PUT_METHOD, request, response)
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

DECLARE_EXECUTOR_METHOD(CRUDExecutor, PATCH_METHOD, request, response)
{
	Database database;
	Table table;
	SQLResult result;
	SQLValue* values = (SQLValue*)malloc(2 * sizeof(SQLValue));
	JSONArray data;
	JSONBuilder builder;

	if (!values)
	{
		raise(SIGSEGV);

		return;
	}

	getDatabaseHTTPRequest(request, "test_database", &database);
	getTable(database, "test_table", &table);
	createJSONBuilder(&builder);

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

	appendJSONBuilderArray(builder, "data", &data);
	setJSONBody(response, builder);

	deleteWebFrameworkSQLValue(values);
	deleteJSONArray(&data);
	deleteWebFrameworkJSONBuider(builder);
}

void initBuffer(size_t size, void* buffer)
{
	JSONArray* array = (JSONArray*)buffer;

	*array = createJSONArray(size);
}

void callback(const char* columnName, const SQLValue columnValue, size_t index, size_t size, void* buffer)
{
	JSONArray* array = (JSONArray*)buffer;
	SQLValueType type;

	getSQLValueType(columnValue, &type);

	if (type == INT_TYPE)
	{
		int64_t result;

		getSQLValueInt(columnValue, &result);

		appendJSONArrayInteger(array, result);
	}
	else if (type == STRING_TYPE)
	{
		const char* result;

		getSQLValueString(columnValue, &result);

		appendJSONArrayString(array, result);
	}
}
