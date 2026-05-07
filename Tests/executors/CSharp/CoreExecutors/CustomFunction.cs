using System.Text.Json.Nodes;
using Framework;

namespace CoreExecutors
{
	public class CustomFunction : IDynamicFunction
	{
		public string Invoke(in JsonObject arguments)
		{
			JsonArray data = arguments["@customFunction"]!.AsObject()["data"]!.AsArray();

			return $"Data: {(int)data[0]!} {(int)data[1]!} {(int)data[2]!}";
		}
	}
}
