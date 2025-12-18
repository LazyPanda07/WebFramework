using Framework;

namespace CoreExecutors
{
	public class CustomFunction : IDynamicFunction
	{
		public string Invoke(List<string> arguments)
		{
			return $"Data: {arguments[0]} {arguments[1]} {arguments[2]}";
		}
	}
}
