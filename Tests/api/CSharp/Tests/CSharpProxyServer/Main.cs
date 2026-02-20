using Framework;
using Framework.Utility;
using System.CommandLine;

class ProxyServer
{
	static int Main(string[] args)
	{
		try
		{
			var configOption = new Option<string>("--config")
			{
				Required = true,
			};
			var useHTTPSOption = new Option<bool>("--useHTTPS")
			{
				DefaultValueFactory = _ => false
			};
			var portOption = new Option<int>("--port")
			{
				Required = true
			};

			RootCommand root =
			[
				configOption,
				useHTTPSOption,
				portOption
			];

			root.SetAction
			(
				parseResult =>
				{
					string config = parseResult.GetRequiredValue(configOption);
					bool useHTTPS = parseResult.GetValue(useHTTPSOption);
					int port = parseResult.GetRequiredValue(portOption);

					Config serverConfig = new(config);

					serverConfig.OverrideConfiguration("useHTTPS", useHTTPS, true);
					serverConfig.OverrideConfiguration("port", port, true);

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
