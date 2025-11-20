namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Data;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json;

public sealed unsafe partial class HttpRequest(nint implementation)
{
	private readonly IntPtr implementation = implementation;

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	internal delegate void InitBufferCallback(nuint size, IntPtr buffer);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	internal delegate void FillBufferCallback(byte[] data, nuint size, IntPtr buffer);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	internal delegate void AddQueryParameterCallback
	(
		[MarshalAs(UnmanagedType.LPUTF8Str)] string key,
		[MarshalAs(UnmanagedType.LPUTF8Str)] string value,
		nuint index,
		IntPtr buffer
	);

	internal struct DynamicPagesVariable
	{
		public IntPtr name;
		public IntPtr value;
	}

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	internal delegate void AddChunkCallback(IntPtr chunk, nuint size, nuint index, IntPtr buffer);

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
	private static unsafe partial void unregisterWFDPFunction(IntPtr implementation, string functionName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	[return: MarshalAs(UnmanagedType.I1)]
	private static unsafe partial bool isWFDPFunctionRegistered(IntPtr implementation, string functionName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getQueryParameters(IntPtr implementation, InitBufferCallback initQueryBuffer, AddQueryParameterCallback addQueryParameter, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getHTTPChunks(IntPtr implementation, InitBufferCallback initChunksBuffer, AddChunkCallback addChunk, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getFile(IntPtr implementation, string filePath, FillBufferCallback fillBuffer, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void processStaticFile(IntPtr implementation, byte[] fileData, nuint size, string fileExtension, FillBufferCallback fillBuffer, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void processWFDPFile(IntPtr implementation, byte[] fileData, nuint size, [In] DynamicPagesVariable[] variables, nuint variablesSize, FillBufferCallback fillBuffer, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getHTTPHeaders(IntPtr implementation, InitBufferCallback initHeadersBuffer, AddQueryParameterCallback addHeader, IntPtr buffer, ref void* exception);

	private static string GetStringData(IntPtr stringImplementation)
	{
		string result = Marshal.PtrToStringUTF8(getDataFromString(stringImplementation))!;

		deleteWebFrameworkString(stringImplementation);

		return result;
	}

	private static string ToCString(string source)
	{
		return source + "\0";
	}

	private static IntPtr AllocateString(string source)
	{
		byte[] bytes = Encoding.UTF8.GetBytes(ToCString(source));
		IntPtr ptr = Marshal.AllocHGlobal(bytes.Length);

		Marshal.Copy(bytes, 0, ptr, bytes.Length);

		return ptr;
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

	public void UnregisterWFDPFunction(string functionName)
	{
		void* exception = null;

		unregisterWFDPFunction(implementation, functionName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

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

	public IDictionary<string, string> GetQueryParameters()
	{
		void* exception = null;
		Dictionary<string, string> queryParameters = [];
		GCHandle handle = GCHandle.Alloc(queryParameters);

		getQueryParameters
		(
			implementation,
			(nuint size, IntPtr buffer) =>
			{
				Dictionary<string, string> queryParameters = (Dictionary<string, string>)GCHandle.FromIntPtr(buffer).Target!;

				queryParameters.EnsureCapacity((int)size);
			},
			(string key, string value, nuint index, IntPtr buffer) =>
			{
				Dictionary<string, string> queryParameters = (Dictionary<string, string>)GCHandle.FromIntPtr(buffer).Target!;

				queryParameters[key] = value;
			},
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		return queryParameters;
	}

	public IList<string> GetChunks()
	{
		void* exception = null;
		List<string> chunks = [];
		GCHandle handle = GCHandle.Alloc(chunks);

		getHTTPChunks
		(
			implementation,
			(nuint size, IntPtr buffer) =>
			{
				List<string> chunks = (List<string>)GCHandle.FromIntPtr(buffer).Target!;

				chunks.EnsureCapacity((int)size);
			},
			(IntPtr chunk, nuint size, nuint index, IntPtr buffer) =>
			{
				List<string> chunks = (List<string>)GCHandle.FromIntPtr(buffer).Target!;

				chunks.Add(Marshal.PtrToStringAuto(chunk, (int)size)!);
			},
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		handle.Free();

		return chunks;
	}

	public byte[] GetFile(string filePath)
	{
		void* exception = null;
		byte[] result = [];
		GCHandle handle = GCHandle.Alloc(result);

		getFile
		(
			implementation,
			filePath,
			(byte[] data, nuint size, IntPtr buffer) =>
			{
				byte[] result = (byte[])GCHandle.FromIntPtr(buffer).Target!;

				result = new byte[(int)size];

				data.CopyTo(result, 0);
			},
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		return result;
	}

	public byte[] ProcessStaticFile(byte[] fileData, string fileExtension)
	{
		void* exception = null;
		byte[] result = [];
		GCHandle handle = GCHandle.Alloc(result);

		processStaticFile
		(
			implementation,
			fileData,
			(nuint)fileData.Length,
			fileExtension,
			(byte[] data, nuint size, IntPtr buffer) =>
			{
				byte[] result = (byte[])GCHandle.FromIntPtr(buffer).Target!;

				result = new byte[(int)size];

				data.CopyTo(result, 0);
			},
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		return result;
	}

	public byte[] ProcessWfdpFile(byte[] fileData, string fileExtension, IDictionary<string, string> variables)
	{
		void* exception = null;
		byte[] result = [];
		GCHandle handle = GCHandle.Alloc(result);
		DynamicPagesVariable[] cvariables = new DynamicPagesVariable[variables.Count];
		int index = 0;

		foreach (var (key, value) in variables)
		{
			cvariables[index++] = new DynamicPagesVariable
			{
				name = AllocateString(key),
				value = AllocateString(value)
			};
		}

		processWFDPFile
		(
			implementation,
			fileData,
			(nuint)fileData.Length,
			cvariables,
			(nuint)cvariables.Length,
			(byte[] data, nuint size, IntPtr buffer) =>
			{
				byte[] result = (byte[])GCHandle.FromIntPtr(buffer).Target!;

				result = new byte[(int)size];

				data.CopyTo(result, 0);
			},
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		foreach (DynamicPagesVariable variable in cvariables)
		{
			Marshal.FreeHGlobal(variable.name);
			Marshal.FreeHGlobal(variable.value);
		}

		return result;
	}

	public IDictionary<string, string> GetHeaders()
	{
		void* exception = null;
		Dictionary<string, string> result = [];
		GCHandle handle = GCHandle.Alloc(result);

		getHTTPHeaders
		(
			implementation,
			(nuint size, IntPtr buffer) =>
			{
				Dictionary<string, string> result = (Dictionary<string, string>)GCHandle.FromIntPtr(buffer).Target!;

				result.EnsureCapacity((int)size);
			},
			(string key, string value, nuint index, IntPtr buffer) =>
			{
				Dictionary<string, string> result = (Dictionary<string, string>)GCHandle.FromIntPtr(buffer).Target!;

				result[key] = value;
			},
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		return result;
	}
}
