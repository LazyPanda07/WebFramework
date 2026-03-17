using Framework;
using Framework.TaskBroker;
using Framework.Utility;
using Framework.Exceptions;

struct CXXTaskSerializer() : IRabbitMqTaskSerializer
{
	public string fileName { get; set; } = "csharp_cxx_internal.txt";
	public readonly string message => "CSharp message";

	public readonly string TaskExecutorName => "CXXTestTaskExecutor";
	public readonly string QueueName => "cxx_queue";
}

struct CCTaskSerializer() : IRabbitMqTaskSerializer
{
	public string fileName { get; set; } = "csharp_cc_internal.txt";
	public readonly string message => "CSharp message";

	public readonly string TaskExecutorName => "c_test_task_executor";
	public readonly string QueueName => "cc_queue";
}

struct PythonTaskSerializer() : IRabbitMqTaskSerializer
{
	public string fileName { get; set; } = "csharp_python_internal.txt";
	public readonly string message => "CSharp message";

	public readonly string TaskExecutorName => "PythonTestTaskExecutor";
	public readonly string QueueName => "python_queue";
}

struct CSharpTaskSerializer() : IRabbitMqTaskSerializer
{
	public string fileName { get; set; } = "csharp_csharp_internal.txt";
	public readonly string message => "CSharp message";

	public readonly string TaskExecutorName => "CSharpTestTaskExecutor";
	public readonly string QueueName => "csharp_queue";
}

public class CSharpExecutor : StatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		string? taskBroker = request.GetJson()["taskBroker"].GetString();

		if (taskBroker == InternalTaskBroker.ImplementationName)
		{
			request.EnqueueTask<InternalTaskBroker, CXXTaskExecutorApi, CXXTaskSerializer>();
			request.EnqueueTask<InternalTaskBroker, CCTaskExecutorApi, CCTaskSerializer>();
			request.EnqueueTask<InternalTaskBroker, PythonTaskExecutorApi, PythonTaskSerializer>();
			request.EnqueueTask<InternalTaskBroker, CSharpTaskExecutorApi, CSharpTaskSerializer>();
		}
		else if (taskBroker == RabbitMqTaskBroker.ImplementationName)
		{
			CXXTaskSerializer cxxTaskSerializer = new()
			{
				fileName = "csharp_cxx_rabbitmq.txt"
			};
			CCTaskSerializer ccTaskSerializer = new()
			{
				fileName = "csharp_cc_rabbitmq.txt"
			};
			PythonTaskSerializer pythonTaskSerializer = new()
			{
				fileName = "csharp_python_rabbitmq.txt"
			};
			CSharpTaskSerializer csharpTaskSerializer = new()
			{
				fileName = "csharp_csharp_rabbitmq.txt"
			};

			request.EnqueueTask<RabbitMqTaskBroker, CXXTaskExecutorApi, CXXTaskSerializer>(ref cxxTaskSerializer);
			request.EnqueueTask<RabbitMqTaskBroker, CCTaskExecutorApi, CCTaskSerializer>(ref ccTaskSerializer);
			request.EnqueueTask<RabbitMqTaskBroker, PythonTaskExecutorApi, PythonTaskSerializer>(ref pythonTaskSerializer);
			request.EnqueueTask<RabbitMqTaskBroker, CSharpTaskExecutorApi, CSharpTaskSerializer>(ref csharpTaskSerializer);
		}
		else
		{
			throw new WebFrameworkApiException($"Wrong taskBroker name: {taskBroker}", ResponseCodes.BadRequest);
		}

		response.SetBody("Accepted");
		response.SetResponseCode(ResponseCodes.Accepted);
	}
}
