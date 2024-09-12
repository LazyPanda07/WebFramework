namespace Framework;

using System.Runtime.InteropServices;

using Framework.Utility;
using Framework.Exceptions;

/// <summary>
/// Web server
/// </summary>
public sealed unsafe partial class WebFramework : IDisposable
{
	private readonly unsafe void* implementation;

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void* createWebFrameworkFromPath(string configPath, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void* createWebFrameworkFromString(string serverConfiguration, string applicationDirectory, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void* createWebFrameworkFromConfig(void* config, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void startWebFrameworkServer(void* implementation, [MarshalAs(UnmanagedType.Bool)] bool wait, IntPtr onStartServer, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void stopWebFrameworkServer(void* implementation, [MarshalAs(UnmanagedType.Bool)] bool wait, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	[return: MarshalAs(UnmanagedType.I1)]
	private static unsafe partial bool isServerRunning(void* implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkObject(void* implementation);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="configPath">Path to *.json config</param>
	/// <exception cref="Exception"></exception>
	/// <exception cref="WebFrameworkException"></exception>
	public WebFramework(string configPath)
	{
		if (!Path.Exists(configPath))
		{
			throw new Exception($"Path {configPath} doesn't exist");
		}

		void* exception = null;

		implementation = createWebFrameworkFromPath(configPath, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="serverConfiguration">*.json config file content </param>
	/// <param name="applicationDirectory">Working directory</param>
	/// <exception cref="WebFrameworkException"></exception>
	public WebFramework(string serverConfiguration, string applicationDirectory)
	{
		void* exception = null;

		implementation = createWebFrameworkFromString(serverConfiguration, applicationDirectory, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="config">Config instance</param>
	/// <exception cref="WebFrameworkException"></exception>
	public WebFramework(Config config)
	{
		void* exception = null;

		implementation = createWebFrameworkFromConfig(config.implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public delegate void OnStartServer();

	/// <summary>
	/// Start server
	/// </summary>
	/// <param name="wait">Wait until server stop</param>
	/// <param name="callback">On start server callback</param>
	/// <exception cref="WebFrameworkException"></exception>
	public void Start(bool wait = false, OnStartServer? callback = null)
	{
		void* exception = null;

		startWebFrameworkServer(implementation, wait, callback == null ? IntPtr.Zero : Marshal.GetFunctionPointerForDelegate(callback), ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// Stop server
	/// </summary>
	/// <param name="wait">Wait until server stop</param>
	/// <exception cref="WebFrameworkException"></exception>
	public void Stop(bool wait = true)
	{
		void* exception = null;

		stopWebFrameworkServer(implementation, wait, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// Is server running
	/// </summary>
	/// <returns></returns>
	/// <exception cref="WebFrameworkException"></exception>
	public bool IsServerRunning()
	{
		void* exception = null;

		bool result = isServerRunning(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}

	public void Dispose() => deleteWebFrameworkObject(implementation);
}
