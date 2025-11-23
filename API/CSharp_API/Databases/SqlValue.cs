namespace Framework;

using Framework.Utility;
using System;
using System.Runtime.InteropServices;

public sealed unsafe partial class SqlValue
{
	internal readonly IntPtr implementation;

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial IntPtr createSQLValue();

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setSQLValueInt(IntPtr implementation, int value);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setSQLValueDouble(IntPtr implementation, double value);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void setSQLValueString(IntPtr implementation, string value);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setSQLValueBool(IntPtr implementation, [MarshalAs(UnmanagedType.Bool)] bool value);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setSQLValueNull(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setSQLValueBlob(IntPtr implementation, byte[] value, nuint size);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial int getSQLValueInt(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial double getSQLValueDouble(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial string getSQLValueString(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	[return: MarshalAs(UnmanagedType.I1)]
	private static unsafe partial bool getSQLValueBool(IntPtr implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void getSQLValueBlob(IntPtr implementation, ref IntPtr result, ref nuint size);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial int getSQLValueType(IntPtr implementation);

	private SqlValue()
	{
		implementation = createSQLValue();
	}

	public SqlValue(int value) :
		this()
	{
		SetValue(value);
	}

	public SqlValue(double value) :
		this()
	{
		SetValue(value);
	}

	public SqlValue(string value) :
		this()
	{
		SetValue(value);
	}

	public SqlValue(bool value) :
		this()
	{
		SetValue(value);
	}

	/// <summary>
	/// Set null value
	/// </summary>
	/// <param name="value"></param>
	public SqlValue(object? value) :
		this()
	{
		SetValue(value);
	}

	public bool IsNull()
	{
		const int nullType = 4;

		return getSQLValueType(implementation) == nullType;
	}

	public void SetInt(int value)
	{
		setSQLValueInt(implementation, value);
	}

	public void SetValue(double value)
	{
		setSQLValueDouble(implementation, value);
	}

	public void SetValue(string value)
	{
		setSQLValueString(implementation, value);
	}

	public void SetValue(bool value)
	{
		setSQLValueBool(implementation, value);
	}

	/// <summary>
	/// Set null value
	/// </summary>
	/// <param name="_"></param>
	public void SetValue(object? _)
	{
		setSQLValueNull(implementation);
	}

	public void SetValue(ReadOnlySpan<byte> value)
	{
		setSQLValueBlob(implementation, value.ToArray(), (nuint)value.Length);
	}

	public byte[] GetValue()
	{
		IntPtr temp = IntPtr.Zero;
		nuint size = 0;

		getSQLValueBlob(implementation, ref temp, ref size);

		byte[] result = new byte[size];

		Marshal.Copy(temp, result, 0, result.Length);

		return result;
	}

	public T GetValue<T>()
	{
		Type type = typeof(T);

		if (type == typeof(object))
		{
			return default!;
		}

		if (type == typeof(float) || type == typeof(double) || type == typeof(decimal))
		{
			return (T)Convert.ChangeType(getSQLValueDouble(implementation), type);
		}

		return type switch
		{
			Type t when t == typeof(bool) => (T)Convert.ChangeType(getSQLValueBool(implementation), type),
			Type t when t == typeof(string) => (T)Convert.ChangeType(getSQLValueString(implementation), type),
			_ => (T)Convert.ChangeType(getSQLValueInt(implementation), type)
		};
	}
}
