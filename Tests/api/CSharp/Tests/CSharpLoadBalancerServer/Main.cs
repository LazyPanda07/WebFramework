using Framework;
using Framework.Utility;

class LoadBalancerServer
{
	static int Main(string config, long port, bool customHeuristic = false, string type = "", bool serversHTTPS = false)
	{
		try
		{
			Config serverConfig = new(config);

			serverConfig.OverrideConfiguration("port", port, true);

			if (customHeuristic)
			{
				serverConfig.OverrideConfiguration("name", "CustomHeuristic", true);
			}

			if (type == "server")
			{
				List<string> settingsPaths = ["load_balancer_web.json"];

				serverConfig.OverrideConfiguration("webServerType", "multiThreaded", true);

				serverConfig.OverrideConfiguration("settingsPaths", settingsPaths, true);
			}
			else
			{
				List<long> listOfServers = [];

				serverConfig.OverrideConfiguration("serversHTTPS", serversHTTPS, true);

				if (serversHTTPS)
				{
					listOfServers =
					[
						10002,
						10003
					];
				}
				else
				{
					listOfServers =
					[
						10000,
						10001,
					];
				}

				serverConfig.OverrideConfiguration("127.0.0.1", listOfServers, true);
			}

			WebFramework server = new(serverConfig);

			server.Start
			(
				true,
				() =>
				{
					using FileStream file = File.OpenWrite($"start_load_balancer_{port}_server.txt");
					using StreamWriter writer = new(file);

					writer.Write($"{Environment.ProcessId}");
				}
			);
		}
		catch (Exception exception)
		{
			Console.WriteLine(exception);

			return -1;
		}

		return 0;
	}
}
