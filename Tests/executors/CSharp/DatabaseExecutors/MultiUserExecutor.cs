using Framework;
using Framework.Databases;
using Framework.Utility;

public class MultiUserExecutor : HeavyOperationStatefulExecutor
{
	private readonly string userId;

	private static Table GetTable(string databaseName, string tableName, HttpRequest request)
	{
		string databaseImplementation = request.GetRouteParameter<string>("databaseImplementation");

		if (databaseImplementation == SqliteDatabase.ImplementationName)
		{
			return request.GetTable<SqliteDatabase>(databaseName, tableName);
		}
		else if (databaseImplementation == PostgresDatabase.ImplementationName)
		{
			return request.GetTable<PostgresDatabase>(databaseName, tableName);
		}

		throw new NotImplementedException($"Wrong databaseImplementation: {databaseImplementation}");
	}

	public MultiUserExecutor()
	{
		userId = Guid.NewGuid().ToString();
	}

	public override void Init(ExecutorSettings settings)
	{
		string withoutPostgresTests = Environment.GetEnvironmentVariable("WITHOUT_POSTGRES_TESTS") ?? "OFF";

		if (withoutPostgresTests == "OFF")
		{
			settings.GetDatabase<PostgresDatabase>("test_database").GetOrCreateTable
			(
				"multi_user",
				"CREATE TABLE IF NOT EXISTS multi_user (id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY, user_id TEXT NOT NULL, data TEXT NOT NULL)"
			);
		}

		settings.GetOrCreateDatabase<SqliteDatabase>("test_database").GetOrCreateTable("multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)");
	}

	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		Table table = GetTable("test_database", "multi_user", request);
		string databaseImplementation = request.GetRouteParameter<string>("databaseImplementation");
		SqlResult result;

		if (databaseImplementation == SqliteDatabase.ImplementationName)
		{
			result = table.ExecuteQuery
			(
				"SELECT * FROM multi_user WHERE user_id = ?",
				WebFrameworkUtility.MakeSqlValues(userId)
			);
		}
		else if (databaseImplementation == PostgresDatabase.ImplementationName)
		{
			result = table.ExecuteQuery
			(
				"SELECT * FROM multi_user WHERE user_id = $1",
				WebFrameworkUtility.MakeSqlValues(userId)
			);
		}
		else
		{
			throw new NotImplementedException($"Not implemented databaseImplementation: {databaseImplementation}");
		}

		List<object> jsonData = [];

		foreach (var row in result)
		{
			jsonData.Add
			(
				new
				{
					id = row["id"].GetValue<int>(),
					user_id = row["user_id"].GetValue<string>(),
					data = row["data"].GetValue<string>()
				}
			);
		}

		response.SetBody
		(
			new
			{
				data = jsonData
			}
		);
	}

	public override void DoPut(HttpRequest request, HttpResponse response)
	{
		Table table = GetTable("test_database", "test_table", request);
		string databaseImplementation = request.GetRouteParameter<string>("databaseImplementation");

		if (databaseImplementation == SqliteDatabase.ImplementationName)
		{
			table.ExecuteQuery
			(
				"INSERT INTO multi_user (user_id, data) VALUES(?, ?)",
				WebFrameworkUtility.MakeSqlValues(userId, request.GetJson()["data"].GetString()!)
			);
		}
		else if (databaseImplementation == PostgresDatabase.ImplementationName)
		{
			table.ExecuteQuery
			(
				"INSERT INTO multi_user (user_id, data) VALUES($1, $2)",
				WebFrameworkUtility.MakeSqlValues(userId, request.GetJson()["data"].GetString()!)
			);
		}
	}
}
