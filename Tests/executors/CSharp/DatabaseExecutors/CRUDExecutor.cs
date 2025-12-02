using Framework;
using Framework.Utility;

public class CRUDExecutor : StatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		Database database = request.GetDatabase("test_database");
		Table table = database.GetTable("test_table");
		SqlResult result = table.ExecuteQuery("SELECT * FROM test_table WHERE name = ?", WebFrameworkUtility.MakeSqlValues("glue"));
		List<object> jsonData = [];

		foreach (var row in result)
		{
			jsonData.Add
			(
				new
				{
					id = row["id"].GetValue<int>(),
					name = row["name"].GetValue<string>(),
					amount = row["amount"].GetValue<int>()
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

	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		request.GetOrCreateDatabase("test_database").GetOrCreateTable
		(
			"test_table",
			"CREATE TABLE IF NOT EXISTS test_table (" +
			"id INTEGER PRIMARY KEY AUTOINCREMENT, " +
			"name VARCHAR(255) NOT NULL, " +
			"amount INTEGER NOT NULL)"
		);
	}

	public override void DoPut(HttpRequest request, HttpResponse response)
	{
		Database database = request.GetDatabase("test_database");
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
		Database database = request.GetDatabase("test_database");
		Table table = database.GetTable("test_table");

		table.ExecuteQuery
		(
			"UPDATE test_table " +
			"SET name = ? " +
			"WHERE amount = ?",
			WebFrameworkUtility.MakeSqlValues("empty", -1)
		);

		SqlResult result = table.ExecuteQuery
		(
			"SELECT * FROM test_table WHERE name = ?",
			WebFrameworkUtility.MakeSqlValues("empty")
		);

		List<object> jsonData = [];

		foreach (var row in result)
		{
			jsonData.Add
			(
				new
				{
					id = row["id"].GetValue<int>(),
					name = row["name"].GetValue<string>(),
					amount = row["amount"].GetValue<int>()
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

	public override void DoDelete(HttpRequest request, HttpResponse response)
	{
		request.GetDatabase("test_database").GetTable("test_table").ExecuteQuery("DROP TABLE test_table");
	}
}
