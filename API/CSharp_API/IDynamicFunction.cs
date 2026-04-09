using System.Text.Json.Nodes;

namespace Framework;

public interface IDynamicFunction
{
	public string Invoke(in JsonObject arguments);
}
