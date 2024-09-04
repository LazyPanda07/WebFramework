namespace Framework.Utility;

using System.Runtime.InteropServices;

using Framework.Exceptions;

/// <summary>
/// Config file representation
/// </summary>
public sealed unsafe partial class Config : IDisposable
{
	public unsafe readonly void* implementation;

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void* createConfigFromPath(string configPath, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void* createConfigFromString(string serverConfiguration, string applicationDirectory, ref void* exception);

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

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void overrideBasePath(void* implementation, string basePath, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void* getConfiguration(void* implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getRawConfiguration(void* implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getDataFromString(void* implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void* getBasePath(void* implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkObject(void* implementation);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="configPath">Path to *.json config file</param>
	/// <exception cref="Exception"></exception>
	/// <exception cref="WebFrameworkException"></exception>
	public Config(string configPath)
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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="serverConfiguration">*.json config file content</param>
	/// <param name="applicationDirectory">Working directory</param>
	/// <exception cref="WebFrameworkException"></exception>
	public Config(string serverConfiguration, string applicationDirectory)
	{
		void* exception = null;

		implementation = createConfigFromString(serverConfiguration, applicationDirectory, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// Override string
	/// </summary>
	/// <param name="key">JSON key</param>
	/// <param name="value">New string value</param>
	/// <param name="recursive">Recursive search for key</param>
	/// <returns>Self</returns>
	/// <exception cref="WebFrameworkException"></exception>
	public Config OverrideConfiguration(string key, string value, bool recursive = false)
	{
		void* exception = null;

		overrideConfigurationString(implementation, key, value, recursive, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	/// <summary>
	/// Override long
	/// </summary>
	/// <param name="key">JSON key</param>
	/// <param name="value">New long value</param>
	/// <param name="recursive">Recursive search for key</param>
	/// <returns>Self</returns>
	/// <exception cref="WebFrameworkException"></exception>
	public Config OverrideConfiguration(string key, long value, bool recursive = false)
	{
		void* exception = null;

		overrideConfigurationInteger(implementation, key, value, recursive, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	/// <summary>
	/// Override bool
	/// </summary>
	/// <param name="key">JSON key</param>
	/// <param name="value">New bool value</param>
	/// <param name="recursive">Recursive search for key</param>
	/// <returns>Self</returns>
	/// <exception cref="WebFrameworkException"></exception>
	public Config OverrideConfiguration(string key, bool value, bool recursive = false)
	{
		void* exception = null;

		overrideConfigurationBoolean(implementation, key, value, recursive, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	/// <summary>
	/// Override string array
	/// </summary>
	/// <param name="key">JSON key</param>
	/// <param name="value">New string array value</param>
	/// <param name="recursive">Recursive search for key</param>
	/// <returns>Self</returns>
	/// <exception cref="WebFrameworkException"></exception>
	public Config OverrideConfiguration(string key, List<string> value, bool recursive = false)
	{
		void* exception = null;

		overrideConfigurationStringArray(implementation, key, [.. value], recursive, value.Count, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	/// <summary>
	/// Override long array
	/// </summary>
	/// <param name="key">JSON</param>
	/// <param name="value">New long array value</param>
	/// <param name="recursive">Recursive search for key</param>
	/// <returns>Self</returns>
	/// <exception cref="WebFrameworkException"></exception>
	public Config OverrideConfiguration(string key, List<long> value, bool recursive = false)
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

	/// <summary>
	/// Override config file directory
	/// </summary>
	/// <param name="basePath">New base path</param>
	/// <returns>Self</returns>
	/// <exception cref="WebFrameworkException"></exception>
	public Config OverrideBasePath(string basePath)
	{
		void* exception = null;

		overrideBasePath(implementation, basePath, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	/// <summary>
	/// Get config file directory
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public string GetConfiguration()
	{
		void* exception = null;

		void* stringPointer = getConfiguration(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		string result = Marshal.PtrToStringUTF8((IntPtr)getDataFromString(stringPointer))!;

		deleteWebFrameworkObject(stringPointer);

		return result;
	}

	/// <summary>
	/// Get raw config JSON string data
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public string GetRawConfiguration()
	{
		void* exception = null;

		char* result = getRawConfiguration(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return Marshal.PtrToStringUTF8((IntPtr)result)!;
	}

	/// <summary>
	/// Get config file directory
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public string GetBasePath()
	{
		void* exception = null;

		void* stringPointer = getBasePath(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		string result = Marshal.PtrToStringUTF8((IntPtr)getDataFromString(stringPointer))!;

		deleteWebFrameworkObject(stringPointer);

		return result;
	}

	public void Dispose() => deleteWebFrameworkObject(implementation);
}
