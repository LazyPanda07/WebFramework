namespace Framework;

using Framework.Utility;

abstract public class HeavyOperationStatefulExecutor : Executor
{
	public override void Destroy()
	{

	}

	public override ExecutorType GetExecutorType()
	{
		return ExecutorType.HeavyOperationStateful;
	}
}
