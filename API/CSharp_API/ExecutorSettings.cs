namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System;
using System.Runtime.InteropServices;
using System.Text.Json;

public sealed unsafe partial class ExecutorSettings(IntPtr implementation)
{
	enum LoadType
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
	private static unsafe partial void deleteWebFrameworkString(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getDataFromString(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getExecutorInitParameters(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial string getExecutorName(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial string getExecutorUserAgentFilter(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial string getExecutorAPIType(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial int getExecutorLoadType(IntPtr implementation, ref void* exception);

	private static string GetStringData(IntPtr stringImplementation)
	{
		string result = Marshal.PtrToStringUTF8(getDataFromString(stringImplementation))!;

		deleteWebFrameworkString(stringImplementation);

		return result;
	}

	public string GetName()
	{
		void* exception = null;
		string result = getExecutorName(implementation, ref exception);

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
		string result = getExecutorUserAgentFilter(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public string GetApiType()
	{
		void* exception = null;
		string result = getExecutorAPIType(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	LoadType GetLoadType()
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
