namespace Framework.Utility;

using System.Runtime.InteropServices;

using Framework.Exceptions;

public partial class Config
{
	public unsafe readonly void* implementation;

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void* createConfigFromPath(string configPath, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void* createConfigFromString(string serverConfiguration, string sourcesPath, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void overrideConfigurationString(void* implementation, string key, string value, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void overrideConfigurationInteger(void* implementation, string key, long value, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void overrideConfigurationBoolean(void* implementation, string key, [MarshalAs(UnmanagedType.Bool)] bool value, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void overrideConfigurationStringArray(void* implementation, string key, [In] string[] value, [MarshalAs(UnmanagedType.Bool)] bool recursive, long size, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void overrideConfigurationIntegerArray(void* implementation, string key, [In] long[] value, [MarshalAs(UnmanagedType.Bool)] bool recursive, long size, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void* getConfigurationString(void* implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getRawConfiguration(void* implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getDataFromString(void* implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void* getBasePath(void* implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkObject(void* implementation);

	public unsafe Config(string configPath)
	{
		if (!Path.Exists(configPath))
		{
			throw new Exception($"Path {configPath} doesn't exist");
		}

		void* exception = null;

		implementation = createConfigFromPath(configPath, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public unsafe Config(string serverConfiguration, string applicationDirectory)
	{
		void* exception = null;

		implementation = createConfigFromString(serverConfiguration, applicationDirectory, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public unsafe Config OverrideConfiguration(string key, string value, bool recursive = false)
	{
		void* exception = null;

		overrideConfigurationString(implementation, key, value, recursive, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	public unsafe Config OverrideConfiguration(string key, long value, bool recursive = false)
	{
		void* exception = null;

		overrideConfigurationInteger(implementation, key, value, recursive, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	public unsafe Config OverrideConfiguration(string key, bool value, bool recursive = false)
	{
		void* exception = null;

		overrideConfigurationBoolean(implementation, key, value, recursive, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	public unsafe Config OverrideConfiguration(string key, List<string> value, bool recursive = false)
	{
		void* exception = null;

		overrideConfigurationStringArray(implementation, key, [.. value], recursive, value.Count, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	public unsafe Config OverrideConfiguration(string key, List<long> value, bool recursive = false)
	{
		void* exception = null;

		fixed (char* keyPointer = key)
		{
			overrideConfigurationIntegerArray(implementation, key, [.. value], recursive, value.Count, ref exception);
		}

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	public unsafe string GetConfiguration()
	{
		void* exception = null;

		void* stringPointer = getConfigurationString(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

#pragma warning disable CS8600 // Converting null literal or possible null value to non-nullable type.
		string result = Marshal.PtrToStringUTF8((IntPtr)getDataFromString(stringPointer));
#pragma warning restore CS8600 // Converting null literal or possible null value to non-nullable type.

		deleteWebFrameworkObject(stringPointer);

#pragma warning disable CS8603 // Possible null reference return.
		return result;
#pragma warning restore CS8603 // Possible null reference return.
	}

	public unsafe string GetRawConfiguration()
	{
		void* exception = null;

		char* result = getRawConfiguration(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

#pragma warning disable CS8603 // Possible null reference return.
		return Marshal.PtrToStringUTF8((IntPtr)result);
#pragma warning restore CS8603 // Possible null reference return.
	}

	public unsafe string GetBasePath()
	{
		void* exception = null;

		void* stringPointer = getBasePath(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

#pragma warning disable CS8600 // Converting null literal or possible null value to non-nullable type.
		string result = Marshal.PtrToStringUTF8((IntPtr)getDataFromString(stringPointer));
#pragma warning restore CS8600 // Converting null literal or possible null value to non-nullable type.

		deleteWebFrameworkObject(stringPointer);

#pragma warning disable CS8603 // Possible null reference return.
		return result;
#pragma warning restore CS8603 // Possible null reference return.
	}

	unsafe ~Config()
	{
		deleteWebFrameworkObject(implementation);
	}
}
