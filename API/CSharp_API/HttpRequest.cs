namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ObjectiveC;
using System.Text;
using System.Text.Json;

public sealed unsafe partial class HttpRequest(nint implementation)
{
	internal readonly IntPtr implementation = implementation;

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void InitBufferCallback(nuint size, IntPtr buffer);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void FillBufferCallback(IntPtr data, nuint size, IntPtr buffer);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void AddKeyValueParameters
	(
		[MarshalAs(UnmanagedType.LPUTF8Str)] string key,
		[MarshalAs(UnmanagedType.LPUTF8Str)] string value,
		nuint index,
		IntPtr buffer
	);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void AddMultipartCallback
	(
		[MarshalAs(UnmanagedType.LPUTF8Str)] string name,
		[MarshalAs(UnmanagedType.LPUTF8Str)] string? fileName,
		[MarshalAs(UnmanagedType.LPUTF8Str)] string? contentType,
		byte[] data,
		nuint index,
		IntPtr buffer
	);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate IntPtr ChunkGeneratorCallback
	(
		IntPtr data
	);

	internal struct DynamicPagesVariable
	{
		public IntPtr name;
		public IntPtr value;
	}

	internal struct InternalLargeData()
	{
		public IntPtr dataPart = IntPtr.Zero;
		public nuint size = 0;
		[MarshalAs(UnmanagedType.Bool)]
		public bool isLastPacket = false;
	}

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void AddChunkCallback(IntPtr chunk, nuint size, nuint index, IntPtr buffer);

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
	private static unsafe partial void registerWFDPFunctionClass(IntPtr implementation, string functionName, string apiType, IntPtr functionClassName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void unregisterWFDPFunction(IntPtr implementation, string functionName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	[return: MarshalAs(UnmanagedType.I1)]
	private static unsafe partial bool isWFDPFunctionRegistered(IntPtr implementation, string functionName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getQueryParameters(IntPtr implementation, InitBufferCallback initQueryBuffer, AddKeyValueParameters addQueryParameter, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getHTTPChunks(IntPtr implementation, InitBufferCallback initChunksBuffer, AddChunkCallback addChunk, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getFile(IntPtr implementation, string filePath, FillBufferCallback fillBuffer, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void processStaticFile(IntPtr implementation, byte[] fileData, nuint size, string fileExtension, FillBufferCallback fillBuffer, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void processWFDPFile(IntPtr implementation, byte[] fileData, nuint size, [In] DynamicPagesVariable[] variables, nuint variablesSize, FillBufferCallback fillBuffer, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getHTTPHeaders(IntPtr implementation, InitBufferCallback initHeadersBuffer, AddKeyValueParameters addHeader, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getCookies(IntPtr implementation, InitBufferCallback initHeadersBuffer, AddKeyValueParameters addCookiee, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getLargeData(IntPtr implementaion, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getMultiparts(IntPtr implementation, InitBufferCallback initMultipartsBuffer, AddMultipartCallback addMultipart, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void sendAssetFile(IntPtr implementation, string filePath, IntPtr response, [In] DynamicPagesVariable[] variables, nuint variablesSize, [MarshalAs(UnmanagedType.Bool)] bool isBinary, string fileName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void sendStaticFile(IntPtr implementation, string filePath, IntPtr response, [MarshalAs(UnmanagedType.Bool)] bool isBinary, string fileName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void sendWFDPFile(IntPtr implementation, string filePath, IntPtr response, [In] DynamicPagesVariable[] variables, nuint variablesSize, [MarshalAs(UnmanagedType.Bool)] bool isBinary, string fileName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void streamFile(IntPtr implementation, string filePath, IntPtr response, string fileName, nuint chunkSize, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial int getRouteIntegerParameter(IntPtr implementation, string routeParameterName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial double getRouteDoubleParameter(IntPtr implementation, string routeParameterName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getRouteStringParameter(IntPtr implementation, string routeParameterName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getOrCreateDatabaseHTTPRequest(IntPtr implementation, string databaseName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getDatabaseHTTPRequest(IntPtr implementation, string databaseName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getOrCreateTableHTTPRequest(IntPtr implementation, string databaseName, string tableName, string createTableQuery, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getTableHTTPRequest(IntPtr implementation, string databaseName, string tableName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void sendFileChunks(IntPtr implementation, IntPtr response, string fileName, ChunkGeneratorCallback generateChunk, IntPtr data, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	internal static unsafe partial void setExceptionData(IntPtr implementation, string errorMessage, int responseCode, string logCategory);

	private static string GetStringData(IntPtr stringImplementation)
	{
		string result = Marshal.PtrToStringUTF8(getDataFromString(stringImplementation))!;

		deleteWebFrameworkString(stringImplementation);

		return result;
	}

	private static void ReadFileDataCallback(IntPtr data, nuint size, IntPtr buffer)
	{
		List<byte> result = (List<byte>)GCHandle.FromIntPtr(buffer).Target!;
		byte[] dataBytes = new byte[(int)size];

		Marshal.Copy(data, dataBytes, 0, dataBytes.Length);

		result.AddRange(dataBytes);
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

	public Dictionary<string, JsonElement> GetJson()
	{
		return GetJson<Dictionary<string, JsonElement>>();
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

	public void RegisterWfdpFunction<T>(string functionName) where T : IDynamicFunction
	{
		void* exception = null;
		string assemblyName = typeof(T).AssemblyQualifiedName!;
		byte[] assemblyBytes = Encoding.UTF8.GetBytes(assemblyName + '\0');
		IntPtr result = Marshal.AllocHGlobal(assemblyBytes.Length);

		Marshal.Copy(assemblyBytes, 0, result, assemblyBytes.Length);

		registerWFDPFunctionClass(implementation, functionName, "csharp", result, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void UnregisterWfdpFunction(string functionName)
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

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

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

		handle.Free();

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return chunks;
	}

	public byte[] GetFile(string filePath)
	{
		void* exception = null;
		List<byte> result = [];
		GCHandle handle = GCHandle.Alloc(result);

		getFile
		(
			implementation,
			filePath,
			ReadFileDataCallback,
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return [.. result];
	}

	public byte[] ProcessStaticFile(byte[] fileData, string fileExtension)
	{
		void* exception = null;
		List<byte> result = [];
		GCHandle handle = GCHandle.Alloc(result);

		processStaticFile
		(
			implementation,
			fileData,
			(nuint)fileData.Length,
			fileExtension,
			ReadFileDataCallback,
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return [.. result];
	}

	public byte[] ProcessWfdpFile(byte[] fileData, IDictionary<string, string>? variables = null)
	{
		void* exception = null;
		List<byte> result = [];
		GCHandle handle = GCHandle.Alloc(result);
		DynamicPagesVariable[] cvariables = new DynamicPagesVariable[variables == null ? 0 : variables.Count];

		if (variables != null)
		{
			int index = 0;

			foreach (var (key, value) in variables)
			{
				cvariables[index++] = new DynamicPagesVariable
				{
					name = AllocateString(key),
					value = AllocateString(value)
				};
			}
		}

		processWFDPFile
		(
			implementation,
			fileData,
			(nuint)fileData.Length,
			cvariables,
			(nuint)cvariables.Length,
			ReadFileDataCallback,
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		foreach (DynamicPagesVariable variable in cvariables)
		{
			Marshal.FreeHGlobal(variable.name);
			Marshal.FreeHGlobal(variable.value);
		}

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return [.. result];
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

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public LargeData GetLargeData()
	{
		void* exception = null;
		IntPtr largeData = getLargeData(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		InternalLargeData data = Marshal.PtrToStructure<InternalLargeData>(largeData);
		LargeData result = new()
		{
			DataPart = new byte[data.size],
			IsLastPacket = data.isLastPacket
		};

		Marshal.Copy(data.dataPart, result.DataPart, 0, result.DataPart.Length);

		return result;
	}

	public IList<Multipart> GetMultiparts()
	{
		void* exception = null;
		List<Multipart> result = [];
		GCHandle handle = GCHandle.Alloc(result);

		getMultiparts
		(
			implementation,
			(nuint size, IntPtr buffer) =>
			{
				List<Multipart> chunks = (List<Multipart>)GCHandle.FromIntPtr(buffer).Target!;

				chunks.EnsureCapacity((int)size);
			},
			(string name, string? fileName, string? contentType, byte[] data, nuint index, IntPtr buffer) =>
			{
				List<Multipart> chunks = (List<Multipart>)GCHandle.FromIntPtr(buffer).Target!;
				Multipart multipart = new()
				{
					Name = name,
					FileName = fileName,
					ContentType = contentType,
					Data = data,
				};

				chunks.Add(multipart);
			},
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public IDictionary<string, string> GetCookies()
	{
		void* exception = null;
		Dictionary<string, string> result = [];
		GCHandle handle = GCHandle.Alloc(result);

		getCookies
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

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public void SendAssetFile(string filePath, HttpResponse response, IDictionary<string, string>? variables = null, bool? isBinary = null, string? fileName = null)
	{
		void* exception = null;
		DynamicPagesVariable[] cvariables = new DynamicPagesVariable[variables == null ? 0 : variables.Count];

		if (variables != null)
		{
			int index = 0;

			foreach (var (key, value) in variables)
			{
				cvariables[index++] = new DynamicPagesVariable
				{
					name = AllocateString(key),
					value = AllocateString(value)
				};
			}
		}

		sendAssetFile
		(
			implementation,
			filePath,
			response.implementation,
			cvariables,
			(nuint)cvariables.Length,
			(bool)(isBinary == null ? true : isBinary),
			fileName ?? "",
			ref exception
		);

		foreach (DynamicPagesVariable variable in cvariables)
		{
			Marshal.FreeHGlobal(variable.name);
			Marshal.FreeHGlobal(variable.value);
		}

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SendStaticFile(string filePath, HttpResponse response, bool? isBinary = null, string? fileName = null)
	{
		void* exception = null;

		sendStaticFile
		(
			implementation,
			filePath,
			response.implementation,
			(bool)(isBinary == null ? true : isBinary),
			fileName ?? "",
			ref exception
		);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SendWfdpFile(string filePath, HttpResponse response, IDictionary<string, string>? variables = null, bool? isBinary = null, string? fileName = null)
	{
		void* exception = null;
		DynamicPagesVariable[] cvariables = new DynamicPagesVariable[variables == null ? 0 : variables.Count];

		if (variables != null)
		{
			int index = 0;

			foreach (var (key, value) in variables)
			{
				cvariables[index++] = new DynamicPagesVariable
				{
					name = AllocateString(key),
					value = AllocateString(value)
				};
			}
		}

		sendWFDPFile
		(
			implementation,
			filePath,
			response.implementation,
			cvariables,
			(nuint)cvariables.Length,
			(bool)(isBinary == null ? true : isBinary),
			(fileName ?? ""),
			ref exception
		);

		foreach (DynamicPagesVariable variable in cvariables)
		{
			Marshal.FreeHGlobal(variable.name);
			Marshal.FreeHGlobal(variable.value);
		}

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void StreamFile(string filePath, HttpResponse response, string? fileName = null, int? chunkSize = null)
	{
		const nuint defaultChunkSize = 14 * 1024 * 1024;

		void* exception = null;

		streamFile(implementation, filePath, response.implementation, fileName ?? "", chunkSize == null ? defaultChunkSize : (nuint)chunkSize, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public T GetRouteParameter<T>(string name)
	{
		void* exception = null;
		object result = typeof(T) switch
		{
			Type t when t == typeof(string) => Marshal.PtrToStringUTF8(getRouteStringParameter(implementation, name, ref exception))!,
			Type t when t.IsPrimitive && t != typeof(float) && t != typeof(double) => Convert.ChangeType(getRouteIntegerParameter(implementation, name, ref exception), typeof(T)),
			Type t when t == typeof(float) || t == typeof(double) => Convert.ChangeType(getRouteDoubleParameter(implementation, name, ref exception), typeof(T)),
			_ => throw new InvalidOperationException($"Wrong route parameter type: {typeof(T).Name}")
		};

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return (T)result;
	}

	public Database GetOrCreateDatabase(string databaseName)
	{
		void* exception = null;
		IntPtr result = getOrCreateDatabaseHTTPRequest(implementation, databaseName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public Database GetDatabase(string databaseName)
	{
		void* exception = null;
		IntPtr result = getDatabaseHTTPRequest(implementation, databaseName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public Table GetOrCreateTable(string databaseName, string tableName, string createTableQuery)
	{
		void* exception = null;
		IntPtr result = getOrCreateTableHTTPRequest(implementation, databaseName, tableName, createTableQuery, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public Table GetTable(string databaseName, string tableName)
	{
		void* exception = null;
		IntPtr result = getTableHTTPRequest(implementation, databaseName, tableName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public void SendChunks(HttpResponse response, ChunkGenerator generator)
	{
		SendFileChunks(response, generator, "");
	}

	public void SendFileChunks(HttpResponse response, ChunkGenerator generator, string fileName)
	{
		void* exception = null;
		GCHandle handle = GCHandle.Alloc(generator);

		sendFileChunks
		(
			implementation,
			response.implementation,
			fileName,
			(IntPtr data) =>
			{
				GCHandle handle = GCHandle.FromIntPtr(data);
				ChunkGenerator generator = (ChunkGenerator)handle.Target!;
				bool finished = false;
				ReadOnlySpan<char> chunk = generator.Generate(ref finished);

				if (finished)
				{
					return IntPtr.Zero;
				}

				byte[] byteChunk = Encoding.UTF8.GetBytes(chunk.ToArray(), 0, chunk.Length);

				if (generator.currentBuffer != IntPtr.Zero)
				{
					Marshal.FreeHGlobal(generator.currentBuffer);
				}

				generator.currentBuffer = Marshal.AllocHGlobal(byteChunk.Length);

				Marshal.Copy(byteChunk, 0, generator.currentBuffer, byteChunk.Length);

				return generator.currentBuffer;
			},
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}
}
