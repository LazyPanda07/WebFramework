namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Collections;
using System.Runtime.InteropServices;

public sealed unsafe partial class SqlResult : IEnumerable<Dictionary<string, SqlValue>>
{
	private readonly List<Dictionary<string, SqlValue>> rows = [];

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void InitBufferCallback(nuint size, IntPtr buffer);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void IterateCallback(IntPtr columnNames, IntPtr columnValues, nuint size, nuint index, IntPtr buffer);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void iterateSQLResult(IntPtr implementation, InitBufferCallback initBuffer, IterateCallback iterate, IntPtr buffer, ref void* exception);

	public SqlResult(IntPtr implementation)
	{
		void* exception = null;
		GCHandle handle = GCHandle.Alloc(rows);

		iterateSQLResult
		(
			implementation,
			(nuint size, IntPtr buffer) =>
			{
				List<Dictionary<string, SqlValue>> rows = (List<Dictionary<string, SqlValue>>)handle.Target!;

				rows.EnsureCapacity((int)size);
			},
			(IntPtr columnNames, IntPtr columnValues, nuint size, nuint index, IntPtr buffer) =>
			{
				int count = (int)size;

				GCHandle handle = GCHandle.FromIntPtr(buffer);
				List<Dictionary<string, SqlValue>> rows = (List<Dictionary<string, SqlValue>>)handle.Target!;
				Dictionary<string, SqlValue> row = [];

				for (int i = 0; i < count; i++)
				{
					IntPtr keyPtr = Marshal.ReadIntPtr(columnNames, i * IntPtr.Size);
					IntPtr valuePtr = Marshal.ReadIntPtr(columnValues, i * IntPtr.Size);

					row[Marshal.PtrToStringUTF8(keyPtr)!] = new(valuePtr);
				}

				rows.Add(row);
			},
			GCHandle.ToIntPtr(handle),
			ref exception
		);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public Dictionary<string, SqlValue> At(int index)
	{
		return rows.ElementAt(index);
	}

	public int Size()
	{
		return rows.Count;
	}

	public Dictionary<string, SqlValue> this[int index]
	{
		get => rows[index];
	}

	public IEnumerator<Dictionary<string, SqlValue>> GetEnumerator()
	{
		foreach (var item in rows)
		{
			yield return item;
		}
	}

	IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
}
