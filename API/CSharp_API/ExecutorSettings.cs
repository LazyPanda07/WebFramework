namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json;

public sealed unsafe partial class ExecutorSettings(IntPtr implementation)
{
	public enum LoadType
	{
		/// <summary>
		/// Create at initialization
		/// </summary>
		Initialization,
		/// <summary>
		/// Create if client connected
		/// </summary>
		Dynamic,
		/// <summary>
		/// Signal that error happened
		/// </summary>
		None
	}

	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.libraryName)]
	private static partial void deleteWebFrameworkString(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void registerDynamicFunctionClassExecutorSettingss(IntPtr implementation, string functionName, string apiType, IntPtr functionClassName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static partial IntPtr getDataFromString(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static partial IntPtr getExecutorInitParameters(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static partial IntPtr getExecutorName(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static partial IntPtr getExecutorUserAgentFilter(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static partial IntPtr getExecutorAPIType(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static partial int getExecutorLoadType(IntPtr implementation, ref void* exception);

	private static string GetStringData(IntPtr stringImplementation)
	{
		string result = Marshal.PtrToStringUTF8(getDataFromString(stringImplementation))!;

		deleteWebFrameworkString(stringImplementation);

		return result;
	}

	/// <summary>
	/// Register function that can be called from .wfdp files
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="functionName"></param>
	/// <exception cref="WebFrameworkException"></exception>
	public void RegisterDynamicFunction<T>(string functionName) where T : IDynamicFunction
	{
		void* exception = null;
		string assemblyName = typeof(T).AssemblyQualifiedName!;
		byte[] assemblyBytes = Encoding.UTF8.GetBytes(assemblyName + '\0');
		IntPtr result = Marshal.AllocHGlobal(assemblyBytes.Length);

		Marshal.Copy(assemblyBytes, 0, result, assemblyBytes.Length);

		registerDynamicFunctionClassExecutorSettingss(implementation, functionName, "csharp", result, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public string GetName()
	{
		void* exception = null;
		string result = Marshal.PtrToStringUTF8(getExecutorName(implementation, ref exception))!;

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public Dictionary<string, JsonElement> GetInitParameters()
	{
		return GetInitParameters<Dictionary<string, JsonElement>>();
	}

	public T GetInitParameters<T>()
	{
		void* exception = null;
		IntPtr parameters = getExecutorInitParameters(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return JsonSerializer.Deserialize<T>(GetStringData(parameters))!;
	}

	public string GetUserAgentFilter()
	{
		void* exception = null;
		string result = Marshal.PtrToStringUTF8(getExecutorUserAgentFilter(implementation, ref exception))!;

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public string GetApiType()
	{
		void* exception = null;
		string result = Marshal.PtrToStringUTF8(getExecutorAPIType(implementation, ref exception))!;

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public LoadType GetLoadType()
	{
		void* exception = null;
		int result = getExecutorLoadType(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return (LoadType)result;
	}
}
