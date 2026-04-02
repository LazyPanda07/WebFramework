namespace Framework.Utility;

using System;
using System.Runtime.InteropServices;

using Framework.Exceptions;

/// <summary>
/// Config file representation
/// </summary>
public sealed partial class Config : IDisposable
{
	public readonly IntPtr implementation;

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr createConfigFromPath(string configPath, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr createConfigFromString(string serverConfiguration, string applicationDirectory, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void overrideConfigurationString(IntPtr implementation, string key, string value, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void overrideConfigurationInteger(IntPtr implementation, string key, long value, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void overrideConfigurationBoolean(IntPtr implementation, string key, [MarshalAs(UnmanagedType.Bool)] bool value, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void overrideConfigurationStringArray(IntPtr implementation, string key, [In] string[] value, [MarshalAs(UnmanagedType.Bool)] bool recursive, long size, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void overrideConfigurationIntegerArray(IntPtr implementation, string key, [In] long[] value, [MarshalAs(UnmanagedType.Bool)] bool recursive, long size, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void overrideBasePath(IntPtr implementation, string basePath, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getConfigurationString(IntPtr implementation, string key, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial long getConfigurationInteger(IntPtr implementation, string key, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	[return: MarshalAs(UnmanagedType.I1)]
	private static partial bool getConfigurationBoolean(IntPtr implementation, string key, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getConfiguration(IntPtr implementation, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getRawConfiguration(IntPtr implementation, ref IntPtr exception); // char*

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getDataFromString(IntPtr implementation);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getBasePath(IntPtr implementation, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteWebFrameworkConfig(IntPtr implementation);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteWebFrameworkString(IntPtr implementation);

	private static string GetStringData(IntPtr stringImplementation)
	{
		string result = Marshal.PtrToStringUTF8(getDataFromString(stringImplementation))!;

		deleteWebFrameworkString(stringImplementation);

		return result;
	}

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

		IntPtr exception = IntPtr.Zero;

		implementation = createConfigFromPath(configPath, ref exception);

		if (exception != IntPtr.Zero)
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
		IntPtr exception = IntPtr.Zero;

		implementation = createConfigFromString(serverConfiguration, applicationDirectory, ref exception);

		if (exception != IntPtr.Zero)
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
	public Config OverrideConfiguration(string key, string value, bool recursive = true)
	{
		IntPtr exception = IntPtr.Zero;

		overrideConfigurationString(implementation, key, value, recursive, ref exception);

		if (exception != IntPtr.Zero)
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
	public Config OverrideConfiguration(string key, long value, bool recursive = true)
	{
		IntPtr exception = IntPtr.Zero;

		overrideConfigurationInteger(implementation, key, value, recursive, ref exception);

		if (exception != IntPtr.Zero)
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
	public Config OverrideConfiguration(string key, bool value, bool recursive = true)
	{
		IntPtr exception = IntPtr.Zero;

		overrideConfigurationBoolean(implementation, key, value, recursive, ref exception);

		if (exception != IntPtr.Zero)
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
	public Config OverrideConfiguration(string key, List<string> value, bool recursive = true)
	{
		IntPtr exception = IntPtr.Zero;

		overrideConfigurationStringArray(implementation, key, [.. value], recursive, value.Count, ref exception);

		if (exception != IntPtr.Zero)
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
	public Config OverrideConfiguration(string key, List<long> value, bool recursive = true)
	{
		IntPtr exception = IntPtr.Zero;

		overrideConfigurationIntegerArray(implementation, key, [.. value], recursive, value.Count, ref exception);

		if (exception != IntPtr.Zero)
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
		IntPtr exception = IntPtr.Zero;

		overrideBasePath(implementation, basePath, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	/// <summary>
	/// Get string from config
	/// </summary>
	/// <param name="key">Config key</param>
	/// <param name="recursive">Search recursively</param>
	/// <returns>Config string value</returns>
	/// <exception cref="WebFrameworkException"></exception>
	public string GetConfigurationString(string key, bool recursive = true)
	{
		IntPtr exception = IntPtr.Zero;

		IntPtr stringPointer = getConfigurationString(implementation, key, recursive, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return GetStringData(stringPointer);
	}

	/// <summary>
	/// Get integer from config
	/// </summary>
	/// <param name="key">Config key</param>
	/// <param name="recursive">Search recursively</param>
	/// <returns>Config integer value</returns>
	/// <exception cref="WebFrameworkException"></exception>
	public long GetConfigurationInteger(string key, bool recursive = true)
	{
		IntPtr exception = IntPtr.Zero;

		long result = getConfigurationInteger(implementation, key, recursive, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	/// <summary>
	/// Get boolean from config
	/// </summary>
	/// <param name="key">Config key</param>
	/// <param name="recursive">Search recursively</param>
	/// <returns>Config boolean value</returns>
	/// <exception cref="WebFrameworkException"></exception>
	public bool GetConfigurationBoolean(string key, bool recursive = true)
	{
		IntPtr exception = IntPtr.Zero;

		bool result = getConfigurationBoolean(implementation, key, recursive, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	/// <summary>
	/// Get config file directory
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public string GetConfiguration()
	{
		IntPtr exception = IntPtr.Zero;

		IntPtr stringPointer = getConfiguration(implementation, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return GetStringData(stringPointer);
	}

	/// <summary>
	/// Get raw config JSON string data
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public string GetRawConfiguration()
	{
		IntPtr exception = IntPtr.Zero;

		string result = Marshal.PtrToStringUTF8(getRawConfiguration(implementation, ref exception))!;

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	/// <summary>
	/// Get config file directory
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public string GetBasePath()
	{
		IntPtr exception = IntPtr.Zero;

		IntPtr stringPointer = getBasePath(implementation, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return GetStringData(stringPointer);
	}

	public void Dispose() => deleteWebFrameworkConfig(implementation);
}
