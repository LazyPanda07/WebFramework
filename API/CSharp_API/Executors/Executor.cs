using Framework.Exceptions;
using System.Reflection;

namespace Framework;

public abstract class Executor
{
	public virtual void Init(ExecutorSettings settings)
	{

	}

	public virtual void DoPost(HttpRequest request, HttpResponse response)
	{
		throw new NotImplementedDoMethodException(nameof(DoPost), GetType());
	}

	public virtual void DoGet(HttpRequest request, HttpResponse response)
	{
		throw new NotImplementedDoMethodException(nameof(DoGet), GetType());
	}

	public virtual void DoHead(HttpRequest request, HttpResponse response)
	{
		throw new NotImplementedDoMethodException(nameof(DoHead), GetType());
	}

	public virtual void DoPut(HttpRequest request, HttpResponse response)
	{
		throw new NotImplementedDoMethodException(nameof(DoPut), GetType());
	}

	public virtual void DoDelete(HttpRequest request, HttpResponse response)
	{
		throw new NotImplementedDoMethodException(nameof(DoDelete), GetType());
	}

	public virtual void DoPatch(HttpRequest request, HttpResponse response)
	{
		throw new NotImplementedDoMethodException(nameof(DoPatch), GetType());
	}

	public virtual void DoOptions(HttpRequest request, HttpResponse response)
	{
#if DEBUG
		List<string> methods = ["OPTIONS"];
		Type type = GetType();
		string allowHeaders = "";

		IsImplemented(methods, type, "POST", nameof(DoPost));
		IsImplemented(methods, type, "GET", nameof(DoGet));
		IsImplemented(methods, type, "HEAD", nameof(DoHead));
		IsImplemented(methods, type, "PUT", nameof(DoPut));
		IsImplemented(methods, type, "DELETE", nameof(DoDelete));
		IsImplemented(methods, type, "PATCH", nameof(DoPatch));
		IsImplemented(methods, type, "TRACE", nameof(DoTrace));
		IsImplemented(methods, type, "CONNECT", nameof(DoConnect));

		for (int i = 0; i < methods.Count; i++)
		{
			allowHeaders += methods[i];

			if (i + 1 != methods.Count)
			{
				allowHeaders += ", ";
			}
		}

		response.AddHeader("Allow", allowHeaders);
#else
		throw new NotImplementedDoMethodException(nameof(DoOptions), GetType());
#endif
	}

	public virtual void DoTrace(HttpRequest request, HttpResponse response)
	{
#if DEBUG
		response.SetBody(request.GetHttpRawRequest());
#else
		throw new NotImplementedDoMethodException(nameof(DoTrace), GetType());
#endif
	}

	public virtual void DoConnect(HttpRequest request, HttpResponse response)
	{
		throw new NotImplementedDoMethodException(nameof(DoConnect), GetType());
	}

	public abstract Utility.ExecutorType GetExecutorType();

	public abstract void Destroy();

	private static void IsImplemented(List<string> methods, Type type, string requestName, string methodName)
	{
		MethodInfo? method = type.GetMethod(methodName, BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);

		if (method == null)
		{
			return;
		}

		if (method.GetBaseDefinition().DeclaringType != method.DeclaringType)
		{
			methods.Add(requestName);
		}
	}
}
