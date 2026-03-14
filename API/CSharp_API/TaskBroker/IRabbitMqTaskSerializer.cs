namespace Framework.TaskBroker;

public interface IRabbitMqTaskSerializer : ITaskSerializer
{
	string QueueName { get; }
}
