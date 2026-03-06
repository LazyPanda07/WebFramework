using Framework;
using Framework.TaskBroker;
using Framework.Utility;

struct CXXTaskSerializer : ITaskSerializer
{
	public readonly string fileName => "csharp_cxx.txt";
	public readonly string message => "CSharp message";

	public readonly string TaskExecutorName => "CXXTestTaskExecutor";
}

struct CCTaskSerializer : ITaskSerializer
{
	public readonly string fileName => "csharp_cc.txt";
	public readonly string message => "CSharp message";

	public readonly string TaskExecutorName => "c_test_task_executor";
}

struct PythonTaskSerializer : ITaskSerializer
{
	public readonly string fileName => "csharp_python.txt";
	public readonly string message => "CSharp message";

	public readonly string TaskExecutorName => "PythonTestTaskExecutor";
}

struct CSharpTaskSerializer : ITaskSerializer
{
	public readonly string fileName => "csharp_csharp.txt";
	public readonly string message => "CSharp message";

	public readonly string TaskExecutorName => "CSharpTestTaskExecutor";
}

public class CSharpExecutor : StatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		request.EnqueueTask<InternalTaskBroker, CXXTaskExecutorApi, CXXTaskSerializer>();

		request.EnqueueTask<InternalTaskBroker, CCTaskExecutorApi, CCTaskSerializer>();

		request.EnqueueTask<InternalTaskBroker, PythonTaskExecutorApi, PythonTaskSerializer>();

		request.EnqueueTask<InternalTaskBroker, CSharpTaskExecutorApi, CSharpTaskSerializer>();

		response.SetBody("Accepted");
		response.SetResponseCode(ResponseCodes.Accepted);
	}
}
