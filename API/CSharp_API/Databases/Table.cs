namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;

public sealed partial class Table(IntPtr implementation)
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr executeQuery(IntPtr implementation, string query, [In] IntPtr[]? values, nuint size, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getTableName(IntPtr implementation, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteSQLResult(IntPtr tableImplementation, IntPtr implementation, ref IntPtr exception);

	public SqlResult ExecuteQuery(string query, IList<SqlValue>? values = null)
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr result;
		
		if (values != null)
		{
			IntPtr[] pointers = new IntPtr[values.Count];

			for (int i = 0; i < values.Count; i++)
			{
				pointers[i] = values[i].implementation;
			}

			result = executeQuery(implementation, query, pointers, (nuint)pointers.Length, ref exception);
		}
		else
		{
			result = executeQuery(implementation, query, null, 0, ref exception);
		}

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		SqlResult sqlResult = new(result);

		deleteSQLResult(implementation, result, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return sqlResult;
	}

	public string GetTableName()
	{
		IntPtr exception = IntPtr.Zero;
		string result = Marshal.PtrToStringUTF8(getTableName(implementation, ref exception))!;

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}
}
