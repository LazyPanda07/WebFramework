#include <APIExecutors/BaseExecutor.h>

static void initBuffer(size_t size, void* buffer);

static void callback(const char* columnName, const SQLValue columnValue, size_t index, size_t size, void* buffer);

DECLARE_DEFAULT_EXECUTOR(CRUDExecutor, STATELESS_EXECUTOR);

DECLARE_EXECUTOR_METHOD(CRUDExecutor, GET_METHOD, request, response)
{
	/*
	framework::Database model = request.getDatabase("test_database");
	framework::Table table = model.getTable("test_table");
	framework::SQLResult result = table.execute("SELECT * FROM test_table WHERE name = ?", { framework::SQLValue("glue") });
	std::vector<framework::JSONObject> data;

	for (const auto& value : result)
	{
		framework::JSONObject object;

		object.setValue("id", value.at("id").get<int64_t>());
		object.setValue("name", value.at("name").get<std::string>());
		object.setValue("amount", value.at("amount").get<int64_t>());

		framework::utility::appendArray(data, object);
	}

	response.setBody(framework::JSONBuilder().append("data", std::move(data)));
	*/

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

	switch (type)
	{
	case INT_TYPE:
		int64_t result;

		getSQLValueInt(columnValue, &result);

		appendJSONArrayInteger(array, result);

		break;

	case STRING_TYPE:
		const char* result;

		getSQLValueString(columnValue, &result);

		appendJSONArrayString(array, result);

		break;

	default:
		break;
	}
}
