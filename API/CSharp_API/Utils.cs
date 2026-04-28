namespace Framework.Utility;

using Framework.Exceptions;
using Framework.TaskBroker;
using System;
using System.Collections.Concurrent;
using System.Linq.Expressions;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json;
using System.Text.Json.Nodes;

public struct LargeData
{
	public byte[] DataPart;
	public bool IsLastPacket;

	public readonly void Deconstruct(out byte[] dataPart, out bool isLastPacket)
	{
		dataPart = DataPart;
		isLastPacket = IsLastPacket;
	}
}

public struct Multipart
{
	public string Name;
	public string? FileName;
	public string? ContentType;
	public byte[] Data;
}

public enum ExecutorType
{
	Stateful,
	Stateless,
	HeavyOperationStateful,
	HeavyOperationStateless
}

public enum ResponseCodes
{
	ContinueCode = 100,
	SwitchingProtocols,
	Processing,
	Ok = 200,
	Created,
	Accepted,
	NonAuthoritativeInformation,
	NoContent,
	ResetContent,
	PartialContent,
	MultiStatus,
	AlreadyReported,
	IMUsed = 226,
	MultipleChoices = 300,
	MovedPermanently,
	Found,
	SeeOther,
	NotModified,
	UseProxy,
	TemporaryRedirect = 307,
	PermanentRedirect,
	BadRequest = 400,
	Unauthorized,
	PaymentRequired,
	Forbidden,
	NotFound,
	MethodNotAllowed,
	NotAcceptable,
	ProxyAuthenticationRequired,
	RequestTimeout,
	Conflict,
	Gone,
	LengthRequired,
	PreconditionFailed,
	PayloadTooLarge,
	URITooLong,
	UnsupportedMediaType,
	RangeNotSatisfiable,
	ExpectationFailed,
	IamATeapot,
	AuthenticationTimeout,
	MisdirectedRequest = 421,
	UnprocessableEntity,
	Locked,
	FailedDependency,
	UpgradeRequired = 426,
	PreconditionRequired = 428,
	TooManyRequests,
	RequestHeaderFieldsTooLarge = 431,
	RetryWith = 449,
	UnavailableForLegalReasons = 451,
	ClientClosedRequest = 499,
	InternalServerError = 500,
	NotImplemented,
	BadGateway,
	ServiceUnavailable,
	GatewayTimeout,
	HTTPVersionNotSupported,
	VariantAlsoNegotiates,
	InsufficientStorage,
	LoopDetected,
	BandwidthLimitExceeded,
	NotExtended,
	NetworkAuthenticationRequired,
	UnknownError = 520,
	WebServerIsDown,
	ConnectionTimedOut,
	OriginIsUnreachable,
	ATimeoutOccurred,
	SSLHandshakeFailed,
	InvalidSSLCertificate
}

public static partial class Utils
{
	private static readonly ConcurrentDictionary<string, Type?> executorTypeCache = [];
	private static readonly ConcurrentDictionary<Type, Func<object>> executorConstructorCache = [];
	private static readonly ConcurrentDictionary<string, Type?> dynamicFunctionTypeCache = [];
	private static readonly ConcurrentDictionary<Type, Func<object>> dynamicFunctionConstructorCache = [];

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr jsonObjectToString(IntPtr implementation, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getDataFromString(IntPtr implementation);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteWebFrameworkString(IntPtr implementation);

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

		try
		{
			method(executorInstance, httpRequestInstance, httpResponseInstance);

			return true;
		}
		catch (WebFrameworkException e)
		{
			HttpRequest.setExceptionData(httpRequestInstance.implementation, e.ToString(), (int)ResponseCodes.InternalServerError, "");

			e.Dispose();

			return false;
		}
		catch (WebFrameworkApiException e)
		{
			HttpRequest.setExceptionData(httpRequestInstance.implementation, e.ToString(), (int)e.ResponseCode, e.LogCategory);

			return false;
		}
		catch (Exception e)
		{
			HttpRequest.setExceptionData(httpRequestInstance.implementation, e.ToString(), (int)ResponseCodes.InternalServerError, "");

			return false;
		}
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

	[UnmanagedCallersOnly(EntryPoint = "HasTaskExecutor")]
	public static int HasTaskExecutor(IntPtr assemblyName)
	{
		string? typeName = Marshal.PtrToStringUTF8(assemblyName);

		if (string.IsNullOrEmpty(typeName))
		{
			return 0;
		}

		Type? targetType = Type.GetType(typeName);

		return targetType == null ? 0 : typeof(ITaskExecutor).IsAssignableFrom(targetType) ? 1 : 0;
	}

	[UnmanagedCallersOnly(EntryPoint = "Free")]
	public static void Free(IntPtr implementation)
	{
		GCHandle.FromIntPtr(implementation).Free();
	}

	[UnmanagedCallersOnly(EntryPoint = "Dealloc")]
	public static void Dealloc(IntPtr implementation)
	{
		Marshal.FreeHGlobal(implementation);
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateExecutor")]
	public static IntPtr CreateExecutor(IntPtr fullName)
	{
		string? typeName = Marshal.PtrToStringUTF8(fullName);

		if (string.IsNullOrEmpty(typeName))
		{
			return IntPtr.Zero;
		}

		Type? type = executorTypeCache.GetOrAdd(typeName, static name => Type.GetType(name, throwOnError: false));

		if (type == null)
		{
			return IntPtr.Zero;
		}

		Func<object> constructor = executorConstructorCache.GetOrAdd
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

		return GCHandle.ToIntPtr(handle);
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateTaskExecutor")]
	public static IntPtr CreateTaskExecutor(IntPtr fullName)
	{
		string? typeName = Marshal.PtrToStringUTF8(fullName);

		if (string.IsNullOrEmpty(typeName))
		{
			return IntPtr.Zero;
		}

		Type? type = Type.GetType(typeName, throwOnError: false);

		if (type == null)
		{
			return IntPtr.Zero;
		}

		NewExpression expression = Expression.New(type);
		object instance = Expression.Lambda<Func<object>>(expression).Compile()();
		GCHandle handle = GCHandle.Alloc(instance);

		return GCHandle.ToIntPtr(handle);
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateExecutorSettings")]
	public static IntPtr CreateExecutorSettings(IntPtr implementation)
	{
		ExecutorSettings request = new(implementation);

		GCHandle handle = GCHandle.Alloc(request);

		return GCHandle.ToIntPtr(handle);
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateHttpRequest")]
	public static IntPtr CreateHttpRequest(IntPtr implementation)
	{
		HttpRequest request = new(implementation);

		GCHandle handle = GCHandle.Alloc(request);

		return GCHandle.ToIntPtr(handle);
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateHttpResponse")]
	public static IntPtr CreateHttpResponse(IntPtr implementation)
	{
		HttpResponse response = new(implementation);

		GCHandle handle = GCHandle.Alloc(response);

		return GCHandle.ToIntPtr(handle);
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateDynamicFunction")]
	public static IntPtr CreateDynamicFunction(IntPtr fullName)
	{
		string? typeName = Marshal.PtrToStringUTF8(fullName);

		if (string.IsNullOrEmpty(typeName))
		{
			return IntPtr.Zero;
		}

		Type? type = dynamicFunctionTypeCache.GetOrAdd(typeName, static name => Type.GetType(name, throwOnError: false));

		if (type == null)
		{
			return IntPtr.Zero;
		}

		Func<object> constructor = dynamicFunctionConstructorCache.GetOrAdd
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

		return GCHandle.ToIntPtr(handle);
	}

	[UnmanagedCallersOnly(EntryPoint = "CreateHeuristic")]
	public static IntPtr CreateHeuristic(IntPtr fullName, IntPtr ip, IntPtr port, int userHTTPS)
	{
		string? typeName = Marshal.PtrToStringUTF8(fullName);

		if (string.IsNullOrEmpty(typeName))
		{
			return IntPtr.Zero;
		}

		Type? type = Type.GetType(typeName, throwOnError: false);

		if (type == null)
		{
			return IntPtr.Zero;
		}

		object instance = Activator.CreateInstance(type, Marshal.PtrToStringUTF8(ip)!, Marshal.PtrToStringUTF8(port)!, Convert.ToBoolean(userHTTPS))!;

		GCHandle handle = GCHandle.Alloc(instance);

		return GCHandle.ToIntPtr(handle);
	}

	[UnmanagedCallersOnly(EntryPoint = "Init")]
	public static void Init(IntPtr executor, IntPtr settings)
	{
		GCHandle executorHandle = GCHandle.FromIntPtr(executor);

		if (executorHandle.Target is not Executor executorInstance)
		{
			return;
		}

		GCHandle settingsHandle = GCHandle.FromIntPtr(settings);

		if (settingsHandle.Target is not ExecutorSettings settingsInstance)
		{
			return;
		}

		executorInstance.Init(settingsInstance);
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

	[UnmanagedCallersOnly(EntryPoint = "CallInvoke")]
	public static IntPtr CallInvoke(IntPtr dynamicFunction, IntPtr arguments)
	{
		GCHandle handle = GCHandle.FromIntPtr(dynamicFunction);

		if (handle.Target is not IDynamicFunction dynamicFunctionInstance)
		{
			return IntPtr.Zero;
		}

		IntPtr exception = IntPtr.Zero;
		IntPtr stringData = jsonObjectToString(arguments, ref exception);

		if (exception != IntPtr.Zero)
		{
			// TODO: Throw exception
			// throw new WebFrameworkException(exception);
		}

		string jsonData = Marshal.PtrToStringUTF8(getDataFromString(stringData))!;
		JsonObject data = JsonNode.Parse(jsonData)!.AsObject();

		deleteWebFrameworkString(stringData);

		string resultString = dynamicFunctionInstance.Invoke(data);
		byte[] resultBytes = Encoding.UTF8.GetBytes(resultString + '\0');
		IntPtr result = Marshal.AllocHGlobal(resultBytes.Length);

		Marshal.Copy(resultBytes, 0, result, resultBytes.Length);

		return result;
	}

	[UnmanagedCallersOnly(EntryPoint = "CallHeuristicOnStart")]
	public static void CallHeuristicOnStart(IntPtr heuristic)
	{
		GCHandle handle = GCHandle.FromIntPtr(heuristic);

		if (handle.Target is not LoadBalancerHeuristic loadBalancerHeuristic)
		{
			return;
		}

		loadBalancerHeuristic.OnStart();
	}

	[UnmanagedCallersOnly(EntryPoint = "CallHeuristicOnEnd")]
	public static void CallHeuristicOnEnd(IntPtr heuristic)
	{
		GCHandle handle = GCHandle.FromIntPtr(heuristic);

		if (handle.Target is not LoadBalancerHeuristic loadBalancerHeuristic)
		{
			return;
		}

		loadBalancerHeuristic.OnEnd();
	}

	[UnmanagedCallersOnly(EntryPoint = "CallHeuristicInvoke")]
	public static nint CallHeuristicInvoke(IntPtr heuristic)
	{
		GCHandle handle = GCHandle.FromIntPtr(heuristic);

		if (handle.Target is not LoadBalancerHeuristic loadBalancerHeuristic)
		{
			return 0;
		}

		return (nint)loadBalancerHeuristic.Invoke();
	}

	[UnmanagedCallersOnly(EntryPoint = "CallTaskExecutorInvoke")]
	public static void CallTaskExecutorInvoke(IntPtr executor, IntPtr jsonObjectData, IntPtr context)
	{
		GCHandle handle = GCHandle.FromIntPtr(executor);
		
		if (handle.Target is not ITaskExecutor taskExecutor)
		{
			return;
		}

		IntPtr exception = IntPtr.Zero;
		IntPtr stringData = jsonObjectToString(jsonObjectData, ref exception);
		
		if (exception != IntPtr.Zero)
		{
			// TODO: Throw exception
			// throw new WebFrameworkException(exception);
		}

		string jsonData = Marshal.PtrToStringUTF8(getDataFromString(stringData))!;
		JsonObject data = JsonNode.Parse(jsonData)!.AsObject();

		deleteWebFrameworkString(stringData);

		taskExecutor.Invoke(data, new(context));
	}
}
