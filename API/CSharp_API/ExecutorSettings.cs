namespace Framework;

using Framework.Databases;
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

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getOrCreateDatabaseExecutorSettings(IntPtr implementation, string databaseName, string implementationName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getDatabaseExecutorSettings(IntPtr implementation, string databaseName, string implementationName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getOrCreateTableExecutorSettings(IntPtr implementation, string databaseName, string implementationName, string tableName, string createTableQuery, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getTableExecutorSettings(IntPtr implementation, string databaseName, string implementationName, string tableName, ref void* exception);

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
		void* exception = null;
		IntPtr result = getOrCreateDatabaseExecutorSettings(implementation, databaseName, T.ImplementationName, ref exception);

		if (exception != null)
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
		void* exception = null;
		IntPtr result = getDatabaseExecutorSettings(implementation, databaseName, T.ImplementationName, ref exception);

		if (exception != null)
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
		void* exception = null;
		IntPtr result = getOrCreateTableExecutorSettings(implementation, databaseName, T.ImplementationName, tableName, createTableQuery, ref exception);

		if (exception != null)
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
		void* exception = null;
		IntPtr result = getTableExecutorSettings(implementation, databaseName, T.ImplementationName, tableName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}
}
