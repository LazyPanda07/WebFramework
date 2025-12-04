using Framework;

public class IdExecutor : HeavyOperationStatelessExecutor
{
	private static int GetProcessId()
	{
		return Environment.ProcessId;
	}

	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		Thread.Sleep(TimeSpan.FromSeconds(10));

		response.SetBody
		(
			new
			{
				id = GetProcessId()
			}
		);
	}
}
