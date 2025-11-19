namespace Framework;

using Framework.Utility;

public class StatefulExecutor : Executor
{
	public override void Destroy()
	{

	}

	public override ExecutorType GetExecutorType()
	{
		return ExecutorType.Stateful;
	}
}
