using Framework;
using Framework.Utility;

class Server
{
	static int Main(string[] args)
	{
		string? runtimes = Environment.GetEnvironmentVariable("RUNTIMES");

		try
		{
			if (Path.Exists("test_database.sqlite"))
			{
				File.Delete("test_database.sqlite");
			}

			Config config = new(args[0]);

			if (!string.IsNullOrEmpty(runtimes))
			{
				if (runtimes.Contains("python", StringComparison.OrdinalIgnoreCase))
				{
					config.OverrideConfiguration("$[]WebFramework.runtimes.0.enabled", true);
				}

				if (runtimes.Contains(".net", StringComparison.OrdinalIgnoreCase))
				{
					config.OverrideConfiguration("$[]WebFramework.runtimes.1.enabled", true);
				}
			}

			WebFramework server = new(config);

			server.Start(true, () => Console.WriteLine("Server is running..."));
		}
		catch (Exception exception)
		{
			Console.WriteLine(exception.Message);

			return -1;
		}

		return 0;
	}
}
