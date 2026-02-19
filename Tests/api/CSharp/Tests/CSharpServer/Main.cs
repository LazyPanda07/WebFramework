using Framework;

class Server
{
	static int Main(string[] args)
	{
		try
		{
			if (Path.Exists("test_database.sqlite"))
			{
				File.Delete("test_database.sqlite");
			}

			WebFramework server = new(args[0]);

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
