namespace Framework;

public abstract class LoadBalancerHeuristic(string ip, string port, bool useHTTPS)
{
	public string Ip { get; } = ip;
	public string Port { get; } = port;
	public bool UseHTTPS { get; } = useHTTPS;

	/// <summary>
	/// Used for load balancing. Lowest value will be picked
	/// </summary>
	/// <returns></returns>
	public abstract ulong Invoke();

	public virtual void OnStart()
	{

	}

	public virtual void OnEnd()
	{

	}
}
