namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;
using System.Text.Json;

public sealed unsafe partial class HttpRequest(nint implementation)
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkString(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getDataFromString(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getJSONParserRawData(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkJSONParser(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getHTTPRawParameters(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getHTTPMethod(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getHTTPVersion(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getHTTPBody(IntPtr implementation, ref nuint bodySize, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void setHTTPAttribute(IntPtr implementation, string name, string value, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getHTTPAttribute(IntPtr implementation, string name, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteHTTPSession(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void removeHTTPAttribute(IntPtr implementation, string name, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getHTTPRequestJSON(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getHTTPRawRequest(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getClientIpV4(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getServerIpV4(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial ushort getClientPort(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial ushort getServerPort(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	[return: MarshalAs(UnmanagedType.I1)]
	private static unsafe partial bool isWFDPFunctionRegistered(IntPtr implementation, string functionName, ref void* exception);

	private static string GetStringData(IntPtr stringImplementation)
	{
		string result = Marshal.PtrToStringUTF8(getDataFromString(stringImplementation))!;

		deleteWebFrameworkString(stringImplementation);

		return result;
	}

	public string GetRawParameters()
	{
		void* exception = null;
		char* result = getHTTPRawParameters(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return Marshal.PtrToStringUTF8((IntPtr)result)!;
	}

	public string GetHttpMethod()
	{
		void* exception = null;
		char* result = getHTTPMethod(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return Marshal.PtrToStringUTF8((IntPtr)result)!;
	}

	public string GetHttpVersion()
	{
		void* exception = null;
		IntPtr result = getHTTPVersion(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return GetStringData(result);
	}

	public string GetHttpBody()
	{
		void* exception = null;
		nuint bodySize = 0;
		char* result = getHTTPBody(implementation, ref bodySize, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return Marshal.PtrToStringUTF8((IntPtr)result, (int)bodySize);
	}

	public void SetHttpAttribute(string name, string value)
	{
		void* exception = null;

		setHTTPAttribute(implementation, name, value, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public string GetHttpAttribute(string name)
	{
		void* exception = null;
		IntPtr result = getHTTPAttribute(implementation, name, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return GetStringData(result);
	}

	public void DeleteHttpSession()
	{
		void* exception = null;

		deleteHTTPSession(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void RemoveHttpAttribute(string name)
	{
		void* exception = null;

		removeHTTPAttribute(implementation, name, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public Dictionary<string, object> GetJson()
	{
		return GetJson<Dictionary<string, object>>();
	}

	public T GetJson<T>()
	{
		void* exception = null;
		IntPtr temp = getHTTPRequestJSON(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		char* jsonDataPtr = getJSONParserRawData(temp, ref exception);

		if (exception != null)
		{
			deleteWebFrameworkJSONParser(temp);

			throw new WebFrameworkException(exception);
		}

		string jsonData = Marshal.PtrToStringUTF8((IntPtr)jsonDataPtr)!;

		deleteWebFrameworkJSONParser(temp);

		T? result = JsonSerializer.Deserialize<T>(jsonData);

		return result == null ? throw new Exception($"Can't deserialize JSON: {jsonData}") : result;
	}

	public string GetHttpRawRequest()
	{
		void* exception = null;
		char* result = getHTTPRawRequest(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return Marshal.PtrToStringUTF8((IntPtr)result)!;
	}

	public string GetClientIpV4()
	{
		void* exception = null;
		IntPtr result = getClientIpV4(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return GetStringData(result);
	}

	public string GetServerIpV4()
	{
		void* exception = null;
		IntPtr result = getServerIpV4(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return GetStringData(result);
	}

	public ushort GetClientPort()
	{
		void* exception = null;
		ushort result = getClientPort(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public ushort GetServerPort()
	{
		void* exception = null;
		ushort result = getServerPort(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	//public void RegisterWfdpFunction()
	//{

	//}

	//public void UnregisterWFDPFunction()
	//{

	//}

	public bool IsWfdpFunctionRegistered(string functionName)
	{
		void* exception = null;
		bool result = isWFDPFunctionRegistered(implementation, functionName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}
}
