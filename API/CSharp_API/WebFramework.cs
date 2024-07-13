namespace Framework;

using System.Runtime.InteropServices;

using Framework.Utility;
using Framework.Exceptions;

public partial class WebFramework
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
	private static unsafe partial void deleteWebFrameworkObject(void* implementation);

	public unsafe WebFramework(string configPath)
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

	public unsafe WebFramework(string serverConfiguration, string applicationDirectory)
	{
		void* exception = null;

		implementation = createWebFrameworkFromString(serverConfiguration, applicationDirectory, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public unsafe WebFramework(Config config)
	{
		void* exception = null;

		implementation = createWebFrameworkFromConfig(config.implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public delegate void OnStartServer();

	public unsafe void Start(bool wait = false, OnStartServer? callback = null)
	{
		void* exception = null;

		startWebFrameworkServer(implementation, wait, callback == null ? IntPtr.Zero : Marshal.GetFunctionPointerForDelegate(callback), ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public unsafe void Stop(bool wait = true)
	{
		void* exception = null;

		stopWebFrameworkServer(implementation, wait, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	unsafe ~WebFramework()
	{
		deleteWebFrameworkObject(implementation);
	}
}
