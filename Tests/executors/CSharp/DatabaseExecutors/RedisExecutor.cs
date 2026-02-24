using Framework;
using Framework.Utility;
using Framework.Databases;

public class RedisExecutor : StatelessExecutor
{
	public override void Init(ExecutorSettings settings)
	{
		string withoutRedisTests = Environment.GetEnvironmentVariable("WITHOUT_REDIS_TESTS") ?? "OFF";

		if (withoutRedisTests == "OFF")
		{
			settings.GetOrCreateDatabase<RedisDatabase>("127.0.0.1:10010:password").GetOrCreateTable("", "");
		}
	}

	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		Table connect = request.GetTable<RedisDatabase>("127.0.0.1:10010:password", "");

		Dictionary<string, object> result = new()
		{
			["string"] = connect.ExecuteQuery("GET", WebFrameworkUtility.MakeSqlValues("string"))[0].First().Value.GetValue<string>(),
			["int"] = connect.ExecuteQuery("GET", WebFrameworkUtility.MakeSqlValues("int"))[0].First().Value.GetValue<int>(),
			["double"] = connect.ExecuteQuery("GET", WebFrameworkUtility.MakeSqlValues("double"))[0].First().Value.GetValue<double>(),
			["bool"] = connect.ExecuteQuery("GET", WebFrameworkUtility.MakeSqlValues("bool"))[0].First().Value.GetValue<bool>(),
		};

		response.SetBody(result);
	}

	public override void DoPut(HttpRequest request, HttpResponse response)
	{
		Table connect = request.GetTable<RedisDatabase>("127.0.0.1:10010:password", "");

		connect.ExecuteQuery("SET", WebFrameworkUtility.MakeSqlValues("string", "qwe"));
		connect.ExecuteQuery("SET", WebFrameworkUtility.MakeSqlValues("int", 5));
		connect.ExecuteQuery("SET", WebFrameworkUtility.MakeSqlValues("double", 2.3));
		connect.ExecuteQuery("SET", WebFrameworkUtility.MakeSqlValues("bool", true));

		response.SetResponseCode(ResponseCodes.Created);
	}
}
