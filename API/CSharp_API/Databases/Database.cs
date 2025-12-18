namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System;
using System.Runtime.InteropServices;

public sealed unsafe partial class Database(IntPtr implementation)
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getOrCreateTable(IntPtr implementation, string tableName, string createTableQuery, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling=StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getTable(IntPtr implementation, string tableName, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	[return: MarshalAs(UnmanagedType.I1)]
	private static unsafe partial bool containsTable(IntPtr implementation, string tableName, ref IntPtr outTable, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getDatabaseName(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getDatabaseFileName(IntPtr implementation, ref void* exception);

	public Table GetOrCreateTable(string tableName, string createTableQuery)
	{
		void* exception = null;
		IntPtr result = getOrCreateTable(implementation, tableName, createTableQuery, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public Table GetTable(string tableName)
	{
		void* exception = null;
		IntPtr result = getTable(implementation, tableName, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public bool Contains(string tableName)
	{
		void* exception = null;
		IntPtr outTable = IntPtr.Zero;
		bool result = containsTable(implementation, tableName, ref outTable, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public bool Contains(string tableName, out Table table)
	{
		void* exception = null;
		IntPtr outTable = IntPtr.Zero;
		bool result = containsTable(implementation, tableName, ref outTable, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		table = new Table(outTable);

		return result;
	}

	public string GetDatabaseName()
	{
		void* exception = null;
		string result = Marshal.PtrToStringUTF8(getDatabaseName(implementation, ref exception))!;

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public string GetDatabaseFileName()
	{
		void* exception = null;
		string result = Marshal.PtrToStringUTF8(getDatabaseFileName(implementation, ref exception))!;

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}
}
