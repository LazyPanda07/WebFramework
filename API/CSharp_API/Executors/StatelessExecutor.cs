namespace Framework;

using Framework.Utility;

abstract public class StatelessExecutor : Executor
{
	public override void Destroy()
	{

	}

	public sealed override ExecutorType GetExecutorType()
	{
		return ExecutorType.Stateless;
	}
}
