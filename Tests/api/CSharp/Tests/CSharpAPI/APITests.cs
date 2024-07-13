using Framework.Utility;
using Newtonsoft.Json;
using System.Text;
using Xunit.Abstractions;

#pragma warning disable CS8602

public class APITests(ITestOutputHelper output)
{
	private readonly ITestOutputHelper output = output;

	private static string GetConfiguration()
	{
		string temp = File.ReadAllText("multi_threaded_config.json");
		StringBuilder result = new();

		foreach (char c in temp)
		{
			if (c == '\r')
			{
				continue;
			}

			result.Append(c);
		}

		return result.ToString();
	}

	private static unsafe Config CreateConfig()
	{
		return new("multi_threaded_config.json");
	}

	[Fact]
	public unsafe void ConfigConstructors()
	{
		Config configFromPath = CreateConfig();
		Config configFromStrings = new(GetConfiguration(), Directory.GetCurrentDirectory());

		Assert.Equal(configFromPath.GetRawConfiguration(), configFromStrings.GetRawConfiguration());
		Assert.Equal(configFromPath.GetConfiguration(), configFromStrings.GetConfiguration());
		Assert.Equal(configFromPath.GetBasePath(), configFromStrings.GetBasePath());
	}

	[Fact]
	public unsafe void ConfigOverrideString()
	{
		Config config = CreateConfig();

		config.OverrideConfiguration("webServerType", "threadPool", true);

		Assert.Contains("threadPool", config.GetConfiguration());
	}

	[Fact]
	public unsafe void ConfigOverrideInteger()
	{
		Config config = CreateConfig();

		config.OverrideConfiguration("cachingSize", 0, true);

		dynamic? data = JsonConvert.DeserializeObject(config.GetConfiguration());

		Assert.NotEqual(data, null);

		long value = (long)data.WebFramework.cachingSize;

		Assert.Equal(0, value);
	}

	[Fact]
	public unsafe void ConfigOverrideBool()
	{
		Config config = CreateConfig();

		config.OverrideConfiguration("usingLogging", false, true);

		dynamic? data = JsonConvert.DeserializeObject(config.GetConfiguration());

		Assert.NotEqual(data, null);

		bool value = (bool)data.Logging.usingLogging;

		Assert.False(value);
	}

	[Fact]
	public unsafe void ConfigOverrideStringArray()
	{
		Config config = CreateConfig();

		config.OverrideConfiguration("loadSources", new List<string>(["anotherSource"]), true);

		dynamic? data = JsonConvert.DeserializeObject(config.GetConfiguration());

		Assert.NotEqual(data, null);

		Assert.Contains("anotherSource", data.WebFramework.loadSources);
	}

	[Fact]
	public unsafe void ConfigOverrideIntegerArray()
	{
		Config config = CreateConfig();

		config.OverrideConfiguration("port", new List<long>([15]), true);

		dynamic? data = JsonConvert.DeserializeObject(config.GetConfiguration());

		Assert.NotEqual(data, null);

		Assert.Contains(15, data.WebServer.port);
	}
}
