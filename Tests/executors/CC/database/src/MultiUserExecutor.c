#include <Executors/Executor.h>

#include <signal.h>

#include <Utility/WebFrameworkUtility.h>

#include "SQLUtility.h"

typedef struct
{
	WebFrameworkString uuid;
} MultiUserExecutor;

DEFINE_EXECUTOR(MultiUserExecutor, HEAVY_OPERATION_STATEFUL_EXECUTOR);

DEFINE_EXECUTOR_INIT(MultiUserExecutor)
{
	MultiUserExecutor* self = (MultiUserExecutor*)executor;

	generateUUID(&self->uuid);
}

DEFINE_EXECUTOR_METHOD(MultiUserExecutor, GET_METHOD, request, response)
{
	MultiUserExecutor* self = (MultiUserExecutor*)executor;
	Table table;
	SQLValue value;
	SQLResult result;
	JSONBuilder builder;
	JSONObject_t data;
	
	getTableHTTPRequest(request, "test_database", "multi_user", &table);
	createSQLValue(&value);
	createJSONBuilder(&builder);

	setSQLValueString(value, getDataFromString(self->uuid));

	executeQuery(table, "SELECT * FROM multi_user WHERE user_id = ?", &value, 1, &result);

	iterateSQLResult(result, initBuffer, callback, &data);

	appendJSONBuilderObject(builder, "data", &data);

	setJSONBody(response, builder);

	deleteJSONObject(&data);
	deleteWebFrameworkSQLValue(value);
	deleteWebFrameworkJSONBuilder(builder);
	deleteSQLResult(table, result);
}

DEFINE_EXECUTOR_METHOD(MultiUserExecutor, POST_METHOD, request, response)
{
	Database database;
	Table table;

	getOrCreateDatabaseHTTPRequest(request, "test_database", &database);
	getOrCreateTable(database, "multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)", &table);
}

DEFINE_EXECUTOR_METHOD(MultiUserExecutor, PUT_METHOD, request, response)
{
	MultiUserExecutor* self = (MultiUserExecutor*)executor;
	Table table;
	SQLValue* values = (SQLValue*)malloc(2 * sizeof(SQLValue));
	SQLResult result;
	JSONParser parser;
	const char* data = NULL;

	if (!values)
	{
		raise(SIGSEGV);

		return;
	}

	getTableHTTPRequest(request, "test_database", "multi_user", &table);
	createSQLValue(&values[0]);
	createSQLValue(&values[1]);
	getHTTPRequestJSON(request, &parser);

	getJSONParserString(parser, "data", true, &data);
	setSQLValueString(values[0], getDataFromString(self->uuid));
	setSQLValueString(values[1], data);

	executeQuery(table, "INSERT INTO multi_user (user_id, data) VALUES(?, ?)", values, 2, &result);

	deleteWebFrameworkSQLValue(values[0]);
	deleteWebFrameworkSQLValue(values[1]);
	deleteSQLResult(table, result);

	free(values);
}

DEFINE_EXECUTOR_DESTROY(MultiUserExecutor)
{
	MultiUserExecutor* self = (MultiUserExecutor*)executor;

	deleteWebFrameworkString(self->uuid);
}
