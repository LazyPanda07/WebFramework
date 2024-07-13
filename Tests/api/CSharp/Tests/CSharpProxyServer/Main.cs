using Framework;

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

			server.Start
			(
				true,
				() =>
				{
					FileStream file;

					switch (port)
					{
						case 15000:
							file = File.OpenWrite("start_proxy_server.txt");

							break;

						case 15001:
							file = File.OpenWrite("start_proxy_https_server.txt");

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
