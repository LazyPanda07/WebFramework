namespace Framework;

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
				serverConfig.OverrideConfiguration("heuristic", "CustomHeuristic", true);
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
					FileStream file;

					switch (port)
					{
						case 9090:
							file = File.OpenWrite("start_load_balancer_9090_server.txt");

							break;

						case 9091:
							file = File.OpenWrite("start_load_balancer_9091_server.txt");

							break;

						case 9092:
							file = File.OpenWrite("start_load_balancer_9092_server.txt");

							break;

						case 9093:
							file = File.OpenWrite("start_load_balancer_9093_server.txt");

							break;

						default:
							return;
					}

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
