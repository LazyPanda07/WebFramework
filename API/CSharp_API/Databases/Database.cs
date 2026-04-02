namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System;
using System.Runtime.InteropServices;

public sealed unsafe partial class Database(IntPtr implementation)
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getOrCreateTable(IntPtr implementation, string tableName, string createTableQuery, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling=StringMarshalling.Utf8)]
	private static partial IntPtr getTable(IntPtr implementation, string tableName, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	[return: MarshalAs(UnmanagedType.I1)]
	private static partial bool containsTable(IntPtr implementation, string tableName, ref IntPtr outTable, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getDatabaseName(IntPtr implementation, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getDatabaseFileName(IntPtr implementation, ref IntPtr exception);

	public Table GetOrCreateTable(string tableName, string createTableQuery)
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr result = getOrCreateTable(implementation, tableName, createTableQuery, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public Table GetTable(string tableName)
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr result = getTable(implementation, tableName, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result);
	}

	public bool Contains(string tableName)
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr outTable = IntPtr.Zero;
		bool result = containsTable(implementation, tableName, ref outTable, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public bool Contains(string tableName, out Table table)
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr outTable = IntPtr.Zero;
		bool result = containsTable(implementation, tableName, ref outTable, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		table = new Table(outTable);

		return result;
	}

	public string GetDatabaseName()
	{
		IntPtr exception = IntPtr.Zero;
		string result = Marshal.PtrToStringUTF8(getDatabaseName(implementation, ref exception))!;

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public string GetDatabaseFileName()
	{
		IntPtr exception = IntPtr.Zero;
		string result = Marshal.PtrToStringUTF8(getDatabaseFileName(implementation, ref exception))!;

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}
}
