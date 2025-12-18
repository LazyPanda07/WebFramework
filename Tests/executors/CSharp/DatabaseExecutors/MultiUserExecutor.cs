using Framework;
using Framework.Utility;

public class MultiUserExecutor : HeavyOperationStatefulExecutor
{
	private readonly string userId;

	public MultiUserExecutor()
	{
		userId = Guid.NewGuid().ToString();
	}

	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		Table table = request.GetTable("test_database", "multi_user");

		SqlResult result = table.ExecuteQuery
		(
			"SELECT * FROM multi_user WHERE user_id = ?",
			WebFrameworkUtility.MakeSqlValues(userId)
		);
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

	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		request.GetOrCreateDatabase("test_database").GetOrCreateTable("multi_user", "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)");
	}

	public override void DoPut(HttpRequest request, HttpResponse response)
	{
		Table table = request.GetTable("test_database", "test_table");

		table.ExecuteQuery
		(
			"INSERT INTO multi_user (user_id, data) VALUES(?, ?)",
			WebFrameworkUtility.MakeSqlValues(userId, request.GetJson()["data"].GetString()!)
		);
	}
}
