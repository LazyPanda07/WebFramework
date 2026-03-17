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

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial char* getWebFrameworkVersion();

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void* createWebFrameworkFromPath(string configPath, ref void* exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void* createWebFrameworkFromString(string serverConfiguration, string applicationDirectory, ref void* exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void* createWebFrameworkFromConfig(void* config, ref void* exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void startWebFrameworkServer(void* implementation, [MarshalAs(UnmanagedType.Bool)] bool wait, IntPtr onStartServer, ref void* exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void kickWebFrameworkServer(void* implementation, string ip, ref void* exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void updateSslCertificatesWebFrameworkServer(void* implementation, ref void* exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void stopWebFrameworkServer(void* implementation, [MarshalAs(UnmanagedType.Bool)] bool wait, ref void* exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	[return: MarshalAs(UnmanagedType.I1)]
	private static partial bool isServerRunning(void* implementation, ref void* exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteWebFramework(void* implementation);

	/// <summary>
	/// Get WebFramework version
	/// </summary>
	/// <returns>String representation of version in format {major}.{minor}.{patch}</returns>
	public static string GetWebFrameworkVersion()
	{
		return Marshal.PtrToStringUTF8((IntPtr)getWebFrameworkVersion())!;
	}

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
	/// Disconnects the client associated with the specified IP address from the web framework server.
	/// </summary>
	/// <param name="ip">The IP address of the client to disconnect. Cannot be null or empty.</param>
	/// <exception cref="WebFrameworkException">Thrown when the server fails to disconnect the specified client.</exception>
	public void Kick(string ip)
	{
		void* exception = null;

		kickWebFrameworkServer(implementation, ip, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	/// <summary>
	/// Updates the SSL certificates used by the web framework server.
	/// </summary>
	/// <remarks>This method applies new SSL certificates to the server. The update may cause active connections to
	/// be reset if certificate changes require a restart. Ensure that the server is in a suitable state before invoking
	/// this method.</remarks>
	/// <exception cref="WebFrameworkException">Thrown if an error occurs while updating the SSL certificates.</exception>
	public void UpdateSslCertificates()
	{
		void* exception = null;

		updateSslCertificatesWebFrameworkServer(implementation, ref exception);

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

	public void Dispose() => deleteWebFramework(implementation);
}
