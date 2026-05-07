using System.Text.Json.Nodes;

using CoreExecutors;
using Framework;

public class AssetsExecutor : StatelessExecutor
{
	public override void Init(ExecutorSettings settings)
	{
		settings.RegisterDynamicFunction<CustomFunction>("customFunction");
	}

	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		IDictionary<string, string> queryParameters = request.GetQueryParameters();
		string fileName = request.GetJson()["fileName"].GetString()!;
		JsonObject arguments = [];

		if (queryParameters.ContainsKey("data"))
		{
			arguments["@print"] = new JsonObject()
			{
				["data"] = queryParameters["data"]
			};
		}
		else if (queryParameters.ContainsKey("first"))
		{
			arguments["@customFunction"] = new JsonObject()
			{
				["data"] = new JsonArray
				(
					int.Parse(queryParameters["first"]),
					int.Parse(queryParameters["second"]),
					int.Parse(queryParameters["third"])
				)
			};
		}

		request.SendDynamicFile($"{fileName}.wfdp", response, arguments);
	}

	public override void DoDelete(HttpRequest request, HttpResponse response)
	{
		request.UnregisterDynamicFunction("customFunction");
	}
}
