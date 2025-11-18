namespace Framework.Utility;

using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Linq.Expressions;
using System.Reflection;
using System.Runtime.InteropServices;

public enum ExecutorType
{
	Stateful,
	Stateless,
	HeavyOperationStateful,
	HeavyOperationStateless
}

public static class Utils
{
	private static readonly ConcurrentDictionary<string, Type?> typeCache = [];
	private static readonly ConcurrentDictionary<Type, Func<object>> constructorCache = [];

	private static bool CallDoMethod(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse, Action<Executor, HttpRequest, HttpResponse> method)
	{
		GCHandle executorHandle = GCHandle.FromIntPtr(executor);

		if (executorHandle.Target is not Executor executorInstance)
		{
			return false;
		}

		GCHandle httpRequestHandle = GCHandle.FromIntPtr(httpRequest);

		if (httpRequestHandle.Target is not HttpRequest httpRequestInstance)
		{
			return false;
		}

		GCHandle httpResponseHandle = GCHandle.FromIntPtr(httpResponse);

		if (httpResponseHandle.Target is not HttpResponse httpResponseInstance)
		{
			return false;
		}

		method(executorInstance, httpRequestInstance, httpResponseInstance);

		return true;
	}

	[UnmanagedCallersOnly(EntryPoint = "HasExecutor")]
	public static int HasExecutor(IntPtr assemblyName)
	{
		string? typeName = Marshal.PtrToStringUTF8(assemblyName);

		if (string.IsNullOrEmpty(typeName))
		{
			return 0;
		}

		Type? targetType = Type.GetType(typeName);

		return targetType == null ? 0 : targetType.IsSubclassOf(typeof(Executor)) ? 1 : 0;
	}

	[UnmanagedCallersOnly(EntryPoint = "Free")]
	public static void Free(IntPtr implementation)
	{
		GCHandle.FromIntPtr(implementation).Free();
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateExecutor")]
	public static IntPtr CreateExecutor(IntPtr fullName)
	{
		string? typeName = Marshal.PtrToStringUTF8(fullName);

		if (string.IsNullOrEmpty(typeName))
		{
			return IntPtr.Zero;
		}

		Type? type = typeCache.GetOrAdd(typeName, static name => Type.GetType(name, throwOnError: false));

		if (type == null)
		{
			return IntPtr.Zero;
		}

		Func<object> constructor = constructorCache.GetOrAdd
		(
			type,
			static type =>
			{
				NewExpression expression = Expression.New(type);

				return Expression.Lambda<Func<object>>(expression).Compile();
			}
		);

		object instance = constructor();

		GCHandle handle = GCHandle.Alloc(instance);

		return (IntPtr)handle;
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateHttpRequest")]
	public static IntPtr CreateHttpRequest(IntPtr implementation)
	{
		HttpRequest request = new(implementation);

		GCHandle handle = GCHandle.Alloc(request);

		return (IntPtr)handle;
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateHttpResponse")]
	public static IntPtr CreateHttpResponse(IntPtr implementation)
	{
		HttpResponse response = new(implementation);

		GCHandle handle = GCHandle.Alloc(response);

		return (IntPtr)handle;
	}

	[UnmanagedCallersOnly(EntryPoint = "CallDoPost")]
	public static int CallDoPost(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse) => CallDoMethod(executor, httpRequest, httpResponse, (instance, request, response) => instance.DoPost(request, response)) ? 1 : 0;

	[UnmanagedCallersOnly(EntryPoint = "CallDoGet")]
	public static int CallDoGet(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse) => CallDoMethod(executor, httpRequest, httpResponse, (instance, request, response) => instance.DoGet(request, response)) ? 1 : 0;

	[UnmanagedCallersOnly(EntryPoint = "CallDoHead")]
	public static int CallDoHead(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse) => CallDoMethod(executor, httpRequest, httpResponse, (instance, request, response) => instance.DoHead(request, response)) ? 1 : 0;

	[UnmanagedCallersOnly(EntryPoint = "CallDoPut")]
	public static int CallDoPut(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse) => CallDoMethod(executor, httpRequest, httpResponse, (instance, request, response) => instance.DoPut(request, response)) ? 1 : 0;

	[UnmanagedCallersOnly(EntryPoint = "CallDoDelete")]
	public static int CallDoDelete(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse) => CallDoMethod(executor, httpRequest, httpResponse, (instance, request, response) => instance.DoDelete(request, response)) ? 1 : 0;

	[UnmanagedCallersOnly(EntryPoint = "CallDoPatch")]
	public static int CallDoPatch(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse) => CallDoMethod(executor, httpRequest, httpResponse, (instance, request, response) => instance.DoPatch(request, response)) ? 1 : 0;

	[UnmanagedCallersOnly(EntryPoint = "CallDoOptions")]
	public static int CallDoOptions(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse) => CallDoMethod(executor, httpRequest, httpResponse, (instance, request, response) => instance.DoOptions(request, response)) ? 1 : 0;

	[UnmanagedCallersOnly(EntryPoint = "CallDoTrace")]
	public static int CallDoTrace(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse) => CallDoMethod(executor, httpRequest, httpResponse, (instance, request, response) => instance.DoTrace(request, response)) ? 1 : 0;

	[UnmanagedCallersOnly(EntryPoint = "CallDoConnect")]
	public static int CallDoConnect(IntPtr executor, IntPtr httpRequest, IntPtr httpResponse) => CallDoMethod(executor, httpRequest, httpResponse, (instance, request, response) => instance.DoConnect(request, response)) ? 1 : 0;

	[UnmanagedCallersOnly(EntryPoint = "GetExecutorType")]
	public static ExecutorType GetExecutorType(IntPtr executor)
	{
		GCHandle executorHandle = GCHandle.FromIntPtr(executor);

		if (executorHandle.Target is not Executor executorInstance)
		{
			throw new NotImplementedException();
		}

		return executorInstance.GetExecutorType();
	}

	[UnmanagedCallersOnly(EntryPoint = "Destroy")]
	public static void Destroy(IntPtr executor)
	{
		GCHandle executorHandle = GCHandle.FromIntPtr(executor);

		if (executorHandle.Target is not Executor executorInstance)
		{
			throw new NotImplementedException();
		}

		executorInstance.Destroy();
	}
}
