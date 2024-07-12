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

			server.Start(true, () =>
			{
				using FileStream stream = File.OpenWrite("start_core_server.txt");
				using StreamWriter writer = new(stream);

				writer.Write($"{Environment.ProcessId}");
			});
		}
		catch (Exception exception)
		{
			Console.WriteLine(exception);

			return -1;
		}

		return 0;
	}
}

/*
int main(int argc, char** argv) try
{
	if (std::filesystem::exists("test_database.sqlite"))
	{
		std::filesystem::remove("test_database.sqlite");
	}

# ifdef CXX_API
	framework::initializeWebFramework("WebFramework");
#endif

	framework::WebFramework server(argv[1]);

	server.start
	(
		true,
		[]()
		{
# ifdef __LINUX__
		pid_t processId = getpid();
#else
		DWORD processId = GetCurrentProcessId();
#endif

		std::ofstream(START_CORE_SERVER_FILE) << processId;
	}
	);

	return 0;
}
catch (const std::exception&e)
{
	std::cerr << e.what() << std::endl;

	exit(-1);
}
*/
