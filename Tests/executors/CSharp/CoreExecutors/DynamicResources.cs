using System.Text.Json.Nodes;

using Framework;

public class DynamicResources : HeavyOperationStatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		byte[] fileData = request.GetFile("page.md");

		response.SetBody(request.ProcessStaticFile(fileData, ".md"));
	}

	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		byte[] fileData = request.GetFile("print.wfdp");
		JsonObject arguments = [];

		arguments["@print"] = new JsonObject()
		{
			["data"] = request.GetJson()["data"].GetString()!
		};

		response.SetBody(request.ProcessDynamicFile(fileData, arguments));
	}
}
