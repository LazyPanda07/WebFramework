using System.Text.Json;

namespace Framework.TaskBroker;

public interface ITaskExecutor
{
	public void Invoke(JsonElement arguments);
}
