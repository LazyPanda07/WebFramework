using Framework.TaskBroker;
using System.Text.Json.Nodes;

class CSharpTestTaskExecutor : ITaskExecutor
{
	public void Invoke(JsonObject arguments)
	{
		using FileStream stream = File.Open(((string?)arguments["fileName"]), FileMode.CreateNew);
		using StreamWriter writer = new(stream);

		writer.Write(((string?)arguments["message"]));
	}
}
