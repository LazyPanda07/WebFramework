using System.Text.Json;

namespace Framework;

public interface IDynamicFunction
{
	public string Invoke(JsonElement arguments);
}
