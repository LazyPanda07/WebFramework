using Framework;

public class DownloadExecutor : HeavyOperationStatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		string fileName = request.GetJson()["fileName"].GetString()!;

		request.StreamFile(fileName, response, fileName);
	}
}
