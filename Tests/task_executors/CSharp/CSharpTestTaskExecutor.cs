using Framework.TaskBroker;
using System.Text.Json.Nodes;

class CSharpTestTaskExecutor : ITaskExecutor
{
	public void Invoke(JsonObject arguments)
	{
		using FileStream stream = File.Open("csharp_test_task_executor.txt", FileMode.CreateNew);
		using StreamWriter writer = new(stream);

		writer.Write(((string?)arguments["message"]));
	}
}
