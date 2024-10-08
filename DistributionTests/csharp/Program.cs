using Framework;

namespace csharp_test
{
	internal class Program
	{
		static void Main(string[] _)
		{
			WebFramework server = new("config.json");

			server.Start(true);
		}
	}
}
