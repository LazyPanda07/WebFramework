namespace Framework.TaskBroker;

using Framework.Databases;
using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;
using System.Text.Json.Nodes;

public sealed partial class TaskExecutorContext(IntPtr implementation)
{
	private readonly IntPtr implementation = implementation;

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void FillBufferCallback(IntPtr data, nuint size, IntPtr buffer);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void getFileTaskExecutorContext(IntPtr implementation, string filePath, FillBufferCallback fillBuffer, IntPtr buffer, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void processStaticFileTaskExecutorContext(IntPtr implementation, byte[] fileData, nuint size, string fileExtension, FillBufferCallback fillBuffer, IntPtr buffer, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void processDynamicFileTaskExecutorContext(IntPtr implementation, byte[] fileData, nuint size, IntPtr arguments, FillBufferCallback fillBuffer, IntPtr buffer, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getOrCreateDatabaseTaskExecutorContext(IntPtr implementation, string databaseName, string implementationName, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getDatabaseTaskExecutorContext(IntPtr implementation, string databaseName, string implementationName, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getTableTaskExecutorContext(IntPtr implementation, string databaseName, string implementationName, string tableName, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getOrCreateTableTaskExecutorContext(IntPtr implementation, string databaseName, string implementationName, string tableName, string createTableQuery, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr createJsonParserFromString(string jsonData, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr createJsonParser(IntPtr jsonParserToCopy, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getJsonParserParsedData(IntPtr implementation, [MarshalAs(UnmanagedType.Bool)] bool weak, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteWebFrameworkJsonParser(IntPtr implementation);

	private static void ReadFileDataCallback(IntPtr data, nuint size, IntPtr buffer)
	{
		List<byte> result = (List<byte>)GCHandle.FromIntPtr(buffer).Target!;
		byte[] dataBytes = new byte[(int)size];

		Marshal.Copy(data, dataBytes, 0, dataBytes.Length);

		result.AddRange(dataBytes);
	}

	/// <summary>
	/// Load file from assets
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public byte[] GetFile(string filePath)
	{
		IntPtr exception = IntPtr.Zero;
		List<byte> result = [];
		GCHandle handle = GCHandle.Alloc(result);

		getFileTaskExecutorContext
		(
			implementation,
			filePath,
			ReadFileDataCallback,
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		if (exception != IntPtr.Zero)
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
		IntPtr exception = IntPtr.Zero;
		List<byte> result = [];
		GCHandle handle = GCHandle.Alloc(result);

		processStaticFileTaskExecutorContext
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

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return [.. result];
	}

	/// <summary>
	/// Process dynamic files such as .wfdp
	/// </summary>
	/// <param name="fileData"></param>
	/// <param name="arguments"></param>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public byte[] ProcessDynamicFile(byte[] fileData, JsonObject? arguments = null)
	{
		IntPtr exception = IntPtr.Zero;
		List<byte> result = [];
		GCHandle handle = GCHandle.Alloc(result);
		IntPtr jsonParser = IntPtr.Zero;
		IntPtr jsonObjectData = IntPtr.Zero;

		if (arguments != null)
		{
			jsonParser = createJsonParserFromString(arguments.ToJsonString(), ref exception);
		}
		else
		{
			jsonParser = createJsonParser(IntPtr.Zero, ref exception);
		}

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		jsonObjectData = getJsonParserParsedData(jsonParser, true, ref exception);

		if (exception != IntPtr.Zero)
		{
			deleteWebFrameworkJsonParser(jsonParser);

			throw new WebFrameworkException(exception);
		}

		processDynamicFileTaskExecutorContext
		(
			implementation,
			fileData,
			(nuint)fileData.Length,
			jsonObjectData,
			ReadFileDataCallback,
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		handle.Free();

		deleteWebFrameworkJsonParser(jsonParser);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return [.. result];
	}

	/// <summary>
	/// Gets an existing database with the specified name or creates a new one if it does not exist.
	/// </summary>
	/// <param name="databaseName">The name of the database to retrieve or create. This parameter cannot be null or empty.</param>
	/// <returns>A Database object representing the specified database. If the database does not exist, a new one is created.</returns>
	public Database GetOrCreateDatabase(string databaseName)
	{
		return GetOrCreateDatabase<DefaultDatabase>(databaseName);
	}

	/// <summary>
	/// Gets an existing database with the specified name or creates a new one using the provided database implementation type.
	/// </summary>
	/// <remarks>This method ensures that a database with the specified name exists, creating it if necessary. The
	/// implementation type parameter determines the behavior and features of the resulting database.</remarks>
	/// <typeparam name="T">The type of the database implementation to use. Must implement the IDatabaseImplementation interface.</typeparam>
	/// <param name="databaseName">The name of the database to retrieve or create. This value cannot be null or empty.</param>
	/// <returns>A Database instance representing the created or retrieved database.</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while attempting to get or create the database.</exception>
	public Database GetOrCreateDatabase<T>(string databaseName) where T : IDatabaseImplementation
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr result = getOrCreateDatabaseTaskExecutorContext(implementation, databaseName, T.ImplementationName, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	/// <summary>
	/// Retrieves the database instance associated with the specified database name.
	/// </summary>
	/// <remarks>Ensure that the database name provided is valid and corresponds to an existing database.</remarks>
	/// <param name="databaseName">The name of the database to retrieve. This parameter cannot be null or empty.</param>
	/// <returns>An instance of the Database class corresponding to the specified database name. Returns null if the database is not
	/// found.</returns>
	public Database GetDatabase(string databaseName)
	{
		return GetDatabase<DefaultDatabase>(databaseName);
	}

	/// <summary>
	/// Retrieves a database instance by name using the specified database implementation
	/// type.
	/// </summary>
	/// <remarks>Ensure that the type parameter T specifies a valid database implementation. This method may throw
	/// an exception if the database cannot be found or accessed.</remarks>
	/// <typeparam name="T">The type of database implementation to use. Must implement the IDatabaseImplementation interface.</typeparam>
	/// <param name="databaseName">The name of the database to retrieve. This parameter cannot be null or empty.</param>
	/// <returns>A Database instance representing the requested database, created using the specified implementation type.</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while attempting to retrieve the database.</exception>
	public Database GetDatabase<T>(string databaseName) where T : IDatabaseImplementation
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr result = getDatabaseTaskExecutorContext(implementation, databaseName, T.ImplementationName, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	/// <summary>
	/// Gets a table with the specified name from the given database, creating it if it does not exist using the provided
	/// SQL query.
	/// </summary>
	/// <remarks>This method ensures that the specified table exists in the database, creating it if necessary. The
	/// caller is responsible for providing a valid SQL query that defines the desired table schema.</remarks>
	/// <param name="databaseName">The name of the database in which to locate or create the table. Cannot be null or empty.</param>
	/// <param name="tableName">The name of the table to retrieve or create. Cannot be null or empty.</param>
	/// <param name="createTableQuery">The SQL query used to create the table if it does not already exist. Must define the table structure and be a valid
	/// SQL statement.</param>
	/// <returns>A Table object representing the existing or newly created table in the specified database.</returns>
	public Table GetOrCreateTable(string databaseName, string tableName, string createTableQuery)
	{
		return GetOrCreateTable<DefaultDatabase>(databaseName, tableName, createTableQuery);
	}

	/// <summary>
	/// Gets an existing table or creates a new table in the specified database using the provided SQL query if the table
	/// does not already exist.
	/// </summary>
	/// <typeparam name="T">The type of database implementation to use. Must implement the IDatabaseImplementation interface.</typeparam>
	/// <param name="databaseName">The name of the database in which to create or retrieve the table.</param>
	/// <param name="tableName">The name of the table to create or retrieve.</param>
	/// <param name="createTableQuery">The SQL query used to create the table if it does not already exist.</param>
	/// <returns>A Table object representing the existing or newly created table in the specified database.</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while attempting to create or retrieve the table.</exception>
	public Table GetOrCreateTable<T>(string databaseName, string tableName, string createTableQuery) where T : IDatabaseImplementation
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr result = getOrCreateTableTaskExecutorContext(implementation, databaseName, T.ImplementationName, tableName, createTableQuery, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	/// <summary>
	/// Retrieves a table with the specified name from the given database.
	/// </summary>
	/// <remarks>Ensure that both the database name and table name are valid and exist in the system to avoid
	/// exceptions.</remarks>
	/// <param name="databaseName">The name of the database from which to retrieve the table. This parameter cannot be null or empty.</param>
	/// <param name="tableName">The name of the table to retrieve. This parameter cannot be null or empty.</param>
	/// <returns>A Table object representing the specified table if it exists in the database; otherwise, null.</returns>
	public Table GetTable(string databaseName, string tableName)
	{
		return GetTable<DefaultDatabase>(databaseName, tableName);
	}

	/// <summary>
	/// Retrieves a table from the specified database using the provided table name.
	/// </summary>
	/// <remarks>Ensure that the database and table names are valid and exist in the specified database.</remarks>
	/// <typeparam name="T">Specifies the type of database implementation to use for the operation. Must implement the IDatabaseImplementation
	/// interface.</typeparam>
	/// <param name="databaseName">The name of the database from which to retrieve the table. This parameter cannot be null or empty.</param>
	/// <param name="tableName">The name of the table to retrieve. This parameter cannot be null or empty.</param>
	/// <returns>A Table object representing the requested table from the database.</returns>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while attempting to retrieve the table.</exception>
	public Table GetTable<T>(string databaseName, string tableName) where T : IDatabaseImplementation
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr result = getTableTaskExecutorContext(implementation, databaseName, T.ImplementationName, tableName, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}
}
