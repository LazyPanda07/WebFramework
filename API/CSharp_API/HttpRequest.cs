namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;
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
		IntPtr name,
		IntPtr fileName,
		IntPtr contentType,
		IntPtr data,
		nuint dataSize,
		nuint index,
		IntPtr buffer
	);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate IntPtr ChunkGeneratorCallback
	(
		IntPtr data,
		IntPtr size
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
		[MarshalAs(UnmanagedType.I1)]
		public bool isLastPacket = false;
	}

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void AddChunkCallback(IntPtr chunk, nuint size, nuint index, IntPtr buffer);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkString(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getDataFromString(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getJsonParserRawData(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkJsonParser(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getRawParameters(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getMethod(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getVersion(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getBody(IntPtr implementation, ref nuint bodySize, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void setAttribute(IntPtr implementation, string name, string value, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getAttribute(IntPtr implementation, string name, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteSession(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void removeAttribute(IntPtr implementation, string name, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getRequestJson(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getRawRequest(IntPtr implementation, ref void* exception);

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
	private static unsafe partial void getChunks(IntPtr implementation, InitBufferCallback initChunksBuffer, AddChunkCallback addChunk, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getFile(IntPtr implementation, string filePath, FillBufferCallback fillBuffer, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void processStaticFile(IntPtr implementation, byte[] fileData, nuint size, string fileExtension, FillBufferCallback fillBuffer, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void processDynamicFile(IntPtr implementation, byte[] fileData, nuint size, [In] DynamicPagesVariable[] variables, nuint variablesSize, FillBufferCallback fillBuffer, IntPtr buffer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void getHeaders(IntPtr implementation, InitBufferCallback initHeadersBuffer, AddKeyValueParameters addHeader, IntPtr buffer, ref void* exception);

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
	private static unsafe partial void sendDynamicFile(IntPtr implementation, string filePath, IntPtr response, [In] DynamicPagesVariable[] variables, nuint variablesSize, [MarshalAs(UnmanagedType.Bool)] bool isBinary, string fileName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void streamFile(IntPtr implementation, string filePath, IntPtr response, string fileName, nuint chunkSize, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial int getRouteIntegerParameter(IntPtr implementation, string routeParameterName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial double getRouteDoubleParameter(IntPtr implementation, string routeParameterName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getRouteStringParameter(IntPtr implementation, string routeParameterName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getOrCreateDatabaseRequest(IntPtr implementation, string databaseName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getDatabaseRequest(IntPtr implementation, string databaseName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getOrCreateTableRequest(IntPtr implementation, string databaseName, string tableName, string createTableQuery, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getTableRequest(IntPtr implementation, string databaseName, string tableName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void sendChunks(IntPtr implementation, IntPtr response, ChunkGeneratorCallback generateChunk, IntPtr data, ref void* exception);

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

	private static IntPtr GenerateChunkCallback(IntPtr data, IntPtr size)
	{
		GCHandle handle = GCHandle.FromIntPtr(data);
		ChunkGenerator generator = (ChunkGenerator)handle.Target!;
		bool finished = false;
		ReadOnlySpan<byte> chunk = generator.Generate(ref finished);

		if (finished)
		{
			return IntPtr.Zero;
		}

		if (generator.currentBuffer != IntPtr.Zero)
		{
			Marshal.FreeHGlobal(generator.currentBuffer);
		}

		generator.currentBuffer = Marshal.AllocHGlobal(chunk.Length);

		fixed (byte* source = chunk)
		{
			byte* destination = (byte*)generator.currentBuffer;

			Buffer.MemoryCopy(source, destination, chunk.Length, chunk.Length);

			Marshal.WriteInt64(size, chunk.Length);
		}

		return generator.currentBuffer;
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

	/// <summary>
	/// Retrieves the raw parameter string associated with the current implementation.
	/// </summary>
	/// <returns>A UTF-8 encoded string containing the raw parameters. The string is never null.</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while retrieving the raw parameters.</exception>
	public string GetRawParameters()
	{
		void* exception = null;
		char* result = getRawParameters(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return Marshal.PtrToStringUTF8((IntPtr)result)!;
	}

	/// <summary>
	/// Gets the HTTP method (such as "GET", "POST", or "PUT") associated with the current request.
	/// </summary>
	/// <returns>A string representing the HTTP method of the request.</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while retrieving the HTTP method.</exception>
	public string GetHttpMethod()
	{
		void* exception = null;
		char* result = getMethod(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return Marshal.PtrToStringUTF8((IntPtr)result)!;
	}

	/// <summary>
	/// Gets the HTTP protocol version used by the current connection.
	/// </summary>
	/// <returns>A string representing the HTTP version (for example, "1.1").</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while retrieving the HTTP version from the underlying implementation.</exception>
	public string GetHttpVersion()
	{
		void* exception = null;
		IntPtr result = getVersion(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return GetStringData(result);
	}

	/// <summary>
	/// Retrieves the body content of the current HTTP request as a UTF-8 encoded string.
	/// </summary>
	/// <returns>A string containing the HTTP request body. Returns an empty string if the request body is empty.</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while retrieving the HTTP request body.</exception>
	public string GetHttpBody()
	{
		void* exception = null;
		nuint bodySize = 0;
		char* result = getBody(implementation, ref bodySize, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return Marshal.PtrToStringUTF8((IntPtr)result, (int)bodySize);
	}

	/// <summary>
	/// Sets an HTTP attribute with the specified name and value for the current context.
	/// </summary>
	/// <param name="name">The name of the HTTP attribute to set. Cannot be null.</param>
	/// <param name="value">The value to assign to the HTTP attribute. Cannot be null.</param>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while setting the HTTP attribute.</exception>
	public void SetHttpAttribute(string name, string value)
	{
		void* exception = null;

		setAttribute(implementation, name, value, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// Retrieves the value of the specified HTTP attribute by name.
	/// </summary>
	/// <param name="name">The name of the HTTP attribute to retrieve. Cannot be null.</param>
	/// <returns>A string containing the value of the specified HTTP attribute, or null if the attribute does not exist.</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while retrieving the HTTP attribute.</exception>
	public string GetHttpAttribute(string name)
	{
		void* exception = null;
		IntPtr result = getAttribute(implementation, name, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return GetStringData(result);
	}

	/// <summary>
	/// Deletes the current HTTP session and releases all associated resources.
	/// </summary>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while deleting the HTTP session.</exception>
	public void DeleteHttpSession()
	{
		void* exception = null;

		deleteSession(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// Removes the HTTP attribute with the specified name from the current object.
	/// </summary>
	/// <param name="name">The name of the HTTP attribute to remove. Cannot be null.</param>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while removing the HTTP attribute.</exception>
	public void RemoveHttpAttribute(string name)
	{
		void* exception = null;

		removeAttribute(implementation, name, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// Retrieves the JSON content as a dictionary of property names and their corresponding JSON elements.
	/// </summary>
	/// <returns>A dictionary containing the JSON property names as keys and their associated <see
	/// cref="System.Text.Json.JsonElement"/> values. The dictionary will be empty if there are no properties.</returns>
	public Dictionary<string, JsonElement> GetJson()
	{
		return GetJson<Dictionary<string, JsonElement>>();
	}

	/// <summary>
	/// Deserializes the JSON response content to an object of the specified type.
	/// </summary>
	/// <typeparam name="T">The type to which the JSON response will be deserialized.</typeparam>
	/// <returns>An instance of type <typeparamref name="T"/> containing the deserialized data from the JSON response.</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while retrieving or parsing the JSON response from the underlying web framework.</exception>
	/// <exception cref="Exception">Thrown if the JSON response cannot be deserialized to the specified type <typeparamref name="T"/>.</exception>
	public T GetJson<T>()
	{
		void* exception = null;
		IntPtr temp = getRequestJson(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		char* jsonDataPtr = getJsonParserRawData(temp, ref exception);

		if (exception != null)
		{
			deleteWebFrameworkJsonParser(temp);

			throw new WebFrameworkException(exception);
		}

		string jsonData = Marshal.PtrToStringUTF8((IntPtr)jsonDataPtr)!;

		deleteWebFrameworkJsonParser(temp);

		T? result = JsonSerializer.Deserialize<T>(jsonData);

		return result == null ? throw new Exception($"Can't deserialize JSON: {jsonData}") : result;
	}

	public string GetHttpRawRequest()
	{
		void* exception = null;
		char* result = getRawRequest(implementation, ref exception);

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

	/// <summary>
	/// Register function that can be called from .wfdp files
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="functionName"></param>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Unregister function that can be used from .wfdp files
	/// </summary>
	/// <param name="functionName"></param>
	/// <exception cref="WebFrameworkException"></exception>
	public void UnregisterWfdpFunction(string functionName)
	{
		void* exception = null;

		unregisterWFDPFunction(implementation, functionName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// Check if function with functionName registered for processing .wfdp files
	/// </summary>
	/// <param name="functionName"></param>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Get HTTP query parameters
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Get HTTP chunks
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public IList<byte[]> GetChunks()
	{
		void* exception = null;
		List<byte[]> chunks = [];
		GCHandle handle = GCHandle.Alloc(chunks);

		getChunks
		(
			implementation,
			(nuint size, IntPtr buffer) =>
			{
				List<byte[]> chunks = (List<byte[]>)GCHandle.FromIntPtr(buffer).Target!;

				chunks.EnsureCapacity((int)size);
			},
			(IntPtr chunk, nuint size, nuint index, IntPtr buffer) =>
			{
				List<byte[]> chunks = (List<byte[]>)GCHandle.FromIntPtr(buffer).Target!;
				byte[] chunkBytes = new byte[(int)size];

				Marshal.Copy(chunk, chunkBytes, 0, chunkBytes.Length);

				chunks.Add(chunkBytes);
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

	/// <summary>
	/// Load file from assets
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Process static file such as .md
	/// </summary>
	/// <param name="fileData"></param>
	/// <param name="fileExtension"></param>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Process dynamic files such as .wfdp
	/// </summary>
	/// <param name="fileData"></param>
	/// <param name="variables"></param>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public byte[] ProcessDynamicFile(byte[] fileData, IDictionary<string, string>? variables = null)
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

		processDynamicFile
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

	/// <summary>
	/// Get HTTP headers
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public IDictionary<string, string> GetHeaders()
	{
		void* exception = null;
		Dictionary<string, string> result = [];
		GCHandle handle = GCHandle.Alloc(result);

		getHeaders
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

	/// <summary>
	/// Get current chunk of data if input file size is too big
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Get multiparts from data from form
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
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
			(IntPtr name, IntPtr fileName, IntPtr contentType, IntPtr data, nuint dataSize, nuint index, IntPtr buffer) =>
			{
				List<Multipart> chunks = (List<Multipart>)GCHandle.FromIntPtr(buffer).Target!;

				Multipart multipart = new()
				{
					Name = Marshal.PtrToStringUTF8(name)!,
					Data = new byte[(int)dataSize],
				};

				if (fileName != IntPtr.Zero)
				{
					multipart.FileName = Marshal.PtrToStringUTF8(fileName);
				}

				if (contentType != IntPtr.Zero)
				{
					multipart.ContentType = Marshal.PtrToStringUTF8(contentType);
				}

				Marshal.Copy(data, multipart.Data, 0, multipart.Data.Length);

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

	/// <summary>
	/// Get cookies
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Send file from assets
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="response"></param>
	/// <param name="variables"></param>
	/// <param name="isBinary"></param>
	/// <param name="fileName"></param>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Send static file from assets
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="response"></param>
	/// <param name="isBinary"></param>
	/// <param name="fileName"></param>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Send dynamic file from assets
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="response"></param>
	/// <param name="variables"></param>
	/// <param name="isBinary"></param>
	/// <param name="fileName"></param>
	/// <exception cref="WebFrameworkException"></exception>
	public void SendDynamicFile(string filePath, HttpResponse response, IDictionary<string, string>? variables = null, bool? isBinary = null, string? fileName = null)
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

		sendDynamicFile
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

	/// <summary>
	/// Stream big file from assets
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="response"></param>
	/// <param name="fileName"></param>
	/// <param name="chunkSize"></param>
	/// <exception cref="WebFrameworkException"></exception>
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

	/// <summary>
	/// Get value from route parameters such as /{string}/{int}/{double}
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	/// <exception cref="InvalidOperationException"></exception>
	/// <exception cref="WebFrameworkException"></exception>
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
		IntPtr result = getOrCreateDatabaseRequest(implementation, databaseName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public Database GetDatabase(string databaseName)
	{
		void* exception = null;
		IntPtr result = getDatabaseRequest(implementation, databaseName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public Table GetOrCreateTable(string databaseName, string tableName, string createTableQuery)
	{
		void* exception = null;
		IntPtr result = getOrCreateTableRequest(implementation, databaseName, tableName, createTableQuery, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public Table GetTable(string databaseName, string tableName)
	{
		void* exception = null;
		IntPtr result = getTableRequest(implementation, databaseName, tableName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public void SendChunks(HttpResponse response, ChunkGenerator generator)
	{
		void* exception = null;
		GCHandle handle = GCHandle.Alloc(generator);

		sendChunks(implementation, response.implementation, GenerateChunkCallback, GCHandle.ToIntPtr(handle), ref exception);

		handle.Free();

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
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
			GenerateChunkCallback,
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
