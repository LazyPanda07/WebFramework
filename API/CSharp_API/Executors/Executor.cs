namespace Framework;

public abstract class Executor
{
	public virtual void Init(ExecutorSettings settings)
	{

	}

	public virtual void DoPost(HttpRequest request, HttpResponse response)
	{

	}

	public virtual void DoGet(HttpRequest request, HttpResponse response)
	{

	}

	public virtual void DoHead(HttpRequest request, HttpResponse response)
	{

	}

	public virtual void DoPut(HttpRequest request, HttpResponse response)
	{

	}

	public virtual void DoDelete(HttpRequest request, HttpResponse response)
	{

	}

	public virtual void DoPatch(HttpRequest request, HttpResponse response)
	{

	}

	public virtual void DoOptions(HttpRequest request, HttpResponse response)
	{

	}

	public virtual void DoTrace(HttpRequest request, HttpResponse response)
	{

	}

	public virtual void DoConnect(HttpRequest request, HttpResponse response)
	{

	}

	public abstract Utility.ExecutorType GetExecutorType();

	public abstract void Destroy();
}
