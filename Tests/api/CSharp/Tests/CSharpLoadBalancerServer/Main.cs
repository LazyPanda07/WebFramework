using Framework;
using Framework.Utility;
using System.CommandLine;

class LoadBalancerServer
{
	static int Main(string[] args)
	{
		string? runtimes = Environment.GetEnvironmentVariable("RUNTIMES");

		try
		{
			var configOption = new Option<string>("--config")
			{
				Required = true
			};
			var portOption = new Option<int>("--port")
			{
				Required = true
			};
			var customHeuristicOption = new Option<bool>("--custom_heuristic")
			{
				DefaultValueFactory = _ => false
			};
			var typeOption = new Option<string>("--type")
			{
				DefaultValueFactory = _ => ""
			};
			var serversHTTPSOption = new Option<bool>("--serversHTTPS")
			{
				DefaultValueFactory = _ => false
			};

			RootCommand root =
			[
				configOption,
				portOption,
				customHeuristicOption,
				typeOption,
				serversHTTPSOption
			];

			root.SetAction
			(
				parseResult =>
				{
					string config = parseResult.GetRequiredValue(configOption);
					int port = parseResult.GetRequiredValue(portOption);
					bool customHeuristic = parseResult.GetValue(customHeuristicOption);
					string? type = parseResult.GetValue(typeOption);
					bool serversHTTPS = parseResult.GetValue(serversHTTPSOption);

					Config serverConfig = new(config);

					serverConfig.OverrideConfiguration("port", port, true);

					if (customHeuristic)
					{
						serverConfig.OverrideConfiguration("$[]LoadBalancer.heuristic.name", "CustomHeuristic", true);
					}

					if (!string.IsNullOrEmpty(runtimes))
					{
						if (runtimes.Contains("python", StringComparison.OrdinalIgnoreCase))
						{
							serverConfig.OverrideConfiguration("$[]WebFramework.runtimes.0.enabled", true);
						}

						if (runtimes.Contains(".net", StringComparison.OrdinalIgnoreCase))
						{
							serverConfig.OverrideConfiguration("$[]WebFramework.runtimes.1.enabled", true);
						}
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

					server.Start(true, () => Console.WriteLine("Server is running..."));
				}
			);

			return root.Parse(args).Invoke();
		}
		catch (Exception exception)
		{
			Console.WriteLine(exception);

			return -1;
		}
	}
}
