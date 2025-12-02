namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;

public sealed unsafe partial class Table(IntPtr implementation)
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr executeQuery(IntPtr implementation, string query, [In] IntPtr[]? values, nuint size, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr getTableName(IntPtr implementation, ref void* exception);

	public SqlResult ExecuteQuery(string query, IList<SqlValue>? values = null)
	{
		void* exception = null;
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

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return new(result, implementation);
	}

	public string GetTableName()
	{
		void* exception = null;
		string result = Marshal.PtrToStringUTF8(getTableName(implementation, ref exception))!;

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}
}
