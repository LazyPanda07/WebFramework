#include <APIExecutors/BaseExecutor.h>

#include <time.h>
#include <signal.h>

static void initBuffer(size_t size, void* buffer);

static void callback(const char** columnNames, const SQLValue* columnValues, size_t size, size_t index, void* buffer);

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

	WebFrameworkString string;

	buildJSONBuilder(builder, &string);

	printf("%s\n", getDataFromString(string));

	setJSONBody(response, builder);

	deleteWebFrameworkSQLValue(value);
	deleteJSONArray(&data);
	deleteWebFrameworkJSONBuider(builder);
	deleteWebFrameworkString(string);
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

	deleteWebFrameworkSQLValue(values[0]);
	deleteWebFrameworkSQLValue(values[1]);
	deleteJSONArray(&data);
	deleteWebFrameworkJSONBuider(builder);
}

void initBuffer(size_t size, void* buffer)
{
	JSONArray* array = (JSONArray*)buffer;

	*array = createJSONArray(size);
}

void callback(const char** columnNames, const SQLValue* columnValues, size_t size, size_t index, void* buffer)
{
	JSONArray* array = (JSONArray*)buffer;
	JSONObject object;

	createJSONObject(&object);

	for (size_t i = 0; i < size; i++)
	{
		SQLValueType type;

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

DEFINE_INITIALIZE_WEB_FRAMEWORK();
