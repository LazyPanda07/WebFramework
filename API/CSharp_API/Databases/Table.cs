namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Collections.Immutable;
using System.Runtime.InteropServices;

public sealed unsafe partial class Table(IntPtr implementation)
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr executeQuery(IntPtr implementation, string query, [In] IntPtr[] values, nuint size, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial string getTableName(IntPtr implementation, ref void* exception);

	public SqlResult ExecuteQuery(string query, ImmutableArray<SqlValue> values)
	{
		void* exception = null;
		IntPtr[] pointers = new IntPtr[values.Length];

		for (int i = 0; i < values.Length; i++)
		{
			pointers[i] = values[i].implementation;
		}

		IntPtr result = executeQuery(implementation, query, pointers, (nuint)pointers.Length, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result, implementation);
	}

	public string GetTableName()
	{
		void* exception = null;
		string result = getTableName(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}
}
