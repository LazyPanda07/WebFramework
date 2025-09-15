using Framework;
using Framework.Exceptions;
using Framework.Utility;

namespace hello_csharp
{
	internal class Program
	{
		static void Main(string[] args)
		{
			try
			{
				using Config config = new("configs/config.json"); // Load config.json

				config.OverrideBasePath("executors"); // Override base path for loading executors

				using WebFramework server = new(config); // Create server

				server.Start(true, () => Console.WriteLine("Server is running at http://127.0.0.1:8080")); // Start server and wait
			}
			catch (WebFrameworkException e)
			{
				Console.WriteLine(e.Message);

				Environment.Exit(-1);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.Message);

				Environment.Exit(-2);
			}
		}
	}
}
