using Framework;

public class DownloadExecutor : HeavyOperationStatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		request.StreamFile("index.html", response, "index.html");
	}
}
