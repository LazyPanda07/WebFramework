using Framework;

public class HelloExecutor : StatelessExecutor
{
	private int value;

	public override void Init(ExecutorSettings executorSettings)
	{
		value = (int)executorSettings.GetInitParameters()["number"];
	}

	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		response.SetBody
		(
			new
			{
				message = "Hello, World!",
				number = value
			}
		);
	}

	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		DoGet(request, response);
	}

	public override void DoHead(HttpRequest request, HttpResponse response)
	{
		DoGet(request, response);
	}

	public override void DoPut(HttpRequest request, HttpResponse response)
	{
		DoGet(request, response);
	}

	public override void DoDelete(HttpRequest request, HttpResponse response)
	{
		DoGet(request, response);
	}

	public override void DoPatch(HttpRequest request, HttpResponse response)
	{
		DoGet(request, response);
	}

	public override void DoConnect(HttpRequest request, HttpResponse response)
	{
		DoGet(request, response);
	}
}
