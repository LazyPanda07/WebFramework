using Framework;

public class CustomHeuristic(string ip, string port) : LoadBalancerHeuristic(ip, port)
{
	private readonly ulong id = GetId();

	public override ulong Invoke()
	{
		return id;
	}

	private static ulong currentId = 0;

	private static ulong GetId()
	{
		return Interlocked.Add(ref currentId, 1);
	}
}
