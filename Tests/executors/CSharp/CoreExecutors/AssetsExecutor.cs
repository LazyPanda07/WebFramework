using CoreExecutors;
using Framework;

public class AssetsExecutor : StatelessExecutor
{
	/*
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		string fileName = request.GetJson()["fileName"].GetString()!;

		request.SendWfdpFile($"{fileName}.wfdp", response, request.GetQueryParameters());
	}
	*/

	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		request.RegisterWfdpFunction<CustomFunction>("customFunction");
	}

	public override void DoDelete(HttpRequest request, HttpResponse response)
	{
		request.UnregisterWfdpFunction("customFunction");
	}
}
