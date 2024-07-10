namespace APITests;

using Framework;

public class API
{
	[Fact]
	public unsafe void Config()
	{
		Config config = new("multi_threaded_config.json");

		Assert.Equal(config.GetConfiguration(), config.GetRawConfiguration());

		config.OverrideConfiguration("settingsPaths", new List<string>{ "load_balancer_web.json" }, true);

		Assert.NotEqual(config.GetConfiguration(), config.GetRawConfiguration());

		Assert.Equal(config.GetBasePath(), Directory.GetCurrentDirectory());
	}
}
