using Framework;
using Framework.Databases;
using Framework.Utility;

struct TableData : IDTO<TableData>
{
	public static TableData Create(IDictionary<string, SqlValue> row)
	{
		TableData result = new()
		{
			id = row["id"].GetValue<int>(),
			name = row["name"].GetValue<string>(),
			amount = row["amount"].GetValue<int>()
		};

		return result;
	}

	public int id;
	public string name;
	public int amount;
}

public class CRUDExecutor : StatelessExecutor
{
	private static Database GetDatabase(string databaseName, HttpRequest request)
	{
		string databaseImplementation = request.GetRouteParameter<string>("databaseImplementation");

		if (databaseImplementation == SqliteDatabase.ImplementationName)
		{
			return request.GetDatabase<SqliteDatabase>(databaseName);
		}
		else if (databaseImplementation == PostgresDatabase.ImplementationName)
		{
			return request.GetDatabase<PostgresDatabase>(databaseName);
		}

		throw new NotImplementedException($"Wrong databaseImplementation: {databaseImplementation}");

		return request.GetDatabase(databaseName);
	}

	public override void Init(ExecutorSettings settings)
	{
		settings.GetOrCreateDatabase("test_database").GetOrCreateTable
		(
			"test_table",
			"CREATE TABLE IF NOT EXISTS test_table (" +
			"id INTEGER PRIMARY KEY AUTOINCREMENT, " +
			"name VARCHAR(255) NOT NULL, " +
			"amount INTEGER NOT NULL)"
		);
	}

	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		response.SetBody
		(
			new
			{
				data = GetDatabase("test_database", request).GetTable("test_table").ExecuteQuery<TableData>("SELECT * FROM test_table WHERE name = ?", WebFrameworkUtility.MakeSqlValues("glue"))
			}
		);
	}

	public override void DoPut(HttpRequest request, HttpResponse response)
	{
		Database database = GetDatabase("test_database", request);
		Table table = database.GetTable("test_table");
		Random random = new((int)DateTime.Now.Ticks);

		table.ExecuteQuery
		(
			"INSERT INTO test_table (name, amount) VALUES(?, ?)",
			WebFrameworkUtility.MakeSqlValues("glue", -1)
		);

		for (int i = 0; i < 10; i++)
		{
			table.ExecuteQuery
			(
				"INSERT INTO test_table (name, amount) VALUES(?, ?)",
				WebFrameworkUtility.MakeSqlValues("glue", random.Next() % 200)
			);
		}
	}

	public override void DoPatch(HttpRequest request, HttpResponse response)
	{
		Database database = GetDatabase("test_database", request);
		Table table = database.GetTable("test_table");

		table.ExecuteQuery
		(
			"UPDATE test_table " +
			"SET name = ? " +
			"WHERE amount = ?",
			WebFrameworkUtility.MakeSqlValues("empty", -1)
		);

		response.SetBody
		(
			new
			{
				data = table.ExecuteQuery<TableData>("SELECT * FROM test_table WHERE name = ?", WebFrameworkUtility.MakeSqlValues("empty"))
			}
		);
	}

	public override void DoDelete(HttpRequest request, HttpResponse response)
	{
		GetDatabase("test_database", request).GetTable("test_table").ExecuteQuery("DROP TABLE test_table");
	}
}
