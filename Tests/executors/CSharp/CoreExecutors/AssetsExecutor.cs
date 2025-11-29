using CoreExecutors;
using Framework;

public class AssetsExecutor : StatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		string fileName = (string)request.GetJson()["fileName"];

		request.SendWfdpFile($"{fileName}.wfdp", response, request.GetQueryParameters());
	}

	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		request.RegisterWfdpFunction<CustomFunction>("customFunction");
	}

	public override void DoDelete(HttpRequest request, HttpResponse response)
	{
		request.UnregisterWfdpFunction("customFunction");
	}
}
