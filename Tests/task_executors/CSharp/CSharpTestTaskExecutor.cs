using Framework.TaskBroker;
using System.Text.Json.Nodes;

class CSharpTestTaskExecutor : ITaskExecutor
{
	public void Invoke(in JsonObject arguments)
	{
		const string header = "From CSharpTestTaskExecutor";

		string? fileName = (string?)arguments["fileName"];
		string? message = (string?)arguments["message"];

		using FileStream stream = File.Open(fileName!, FileMode.CreateNew);
		using StreamWriter writer = new(stream);

		writer.WriteLine(header);
		writer.WriteLine(message!);
	}
}
