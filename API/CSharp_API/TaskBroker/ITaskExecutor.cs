using System.Text.Json.Nodes;

namespace Framework.TaskBroker;

public interface ITaskExecutor
{
	public void Invoke(in JsonObject arguments, TaskExecutorContext context);
}
