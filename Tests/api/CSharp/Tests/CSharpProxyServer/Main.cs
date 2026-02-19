using Framework;
using Framework.Utility;

class ProxyServer
{
	static int Main(string config, long port, bool useHTTPS = false)
	{
		try
		{
			Config serverConfig = new(config);

			serverConfig.OverrideConfiguration("useHTTPS", useHTTPS, true);
			serverConfig.OverrideConfiguration("port", port, true);

			WebFramework server = new(serverConfig);

			server.Start(true, () => Console.WriteLine("Server is running..."));
		}
		catch (Exception exception)
		{
			Console.WriteLine(exception);

			return -1;
		}

		return 0;
	}
}
