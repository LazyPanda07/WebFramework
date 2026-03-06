using Framework;

public class IdExecutor : HeavyOperationStatelessExecutor
{
	private static int GetProcessId()
	{
		return Environment.ProcessId;
	}

	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		response.SetBody
		(
			new
			{
				id = GetProcessId()
			}
		);
	}
}
