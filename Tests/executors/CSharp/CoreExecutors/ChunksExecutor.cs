using CoreExecutors;
using Framework;

public class ChunksExecutor : HeavyOperationStatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		TextGenerator textGenerator = new("Some information here");

		request.SendChunks(response, textGenerator);
	}

	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		TextGenerator textGenerator = new("Some information here");

		request.SendFileChunks(response, textGenerator, "file.txt");
	}
}
