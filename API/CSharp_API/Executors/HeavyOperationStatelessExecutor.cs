using Framework.Utility;

namespace Framework;

public class HeavyOperationStatelessExecutor : Executor
{
	public sealed override void Destroy()
	{

	}

	public override ExecutorType GetExecutorType()
	{
		return ExecutorType.HeavyOperationStateless;
	}
}
