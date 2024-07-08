using System.Runtime.InteropServices;

namespace Framework
{
	public partial class WebFramework
	{
		private readonly unsafe void* implementation;

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void* createWebFrameworkFromPath(char* configPath, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void* createWebFrameworkFromString(char* serverConfiguration, char* sourcesPath, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void* createWebFrameworkFromConfig(void* config, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void startWebFrameworkServer(void* implementation, [MarshalAs(UnmanagedType.Bool)] bool wait, IntPtr onStartServer, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void stopWebFrameworkServer(void* implementation, [MarshalAs(UnmanagedType.Bool)] bool wait, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void free(void* implementation);


		unsafe WebFramework(string configPath)
		{
			if (!Path.Exists(configPath))
			{
				throw new Exception($"Path {configPath} doesn't exist");
			}

			void* exception = null;

			fixed (char* configPathPointer = configPath)
			{
				implementation = createWebFrameworkFromPath(configPathPointer, ref exception);
			}

			if (exception != null)
			{
				throw new WebFrameworkException(exception);
			}
		}

		unsafe WebFramework(string serverConfiguration, string sourcesPath)
		{
			void* exception = null;

			fixed (char* serverConfigurationPointer = serverConfiguration)
			{
				fixed (char* sourcesPathPointer = sourcesPath)
				{
					implementation = createWebFrameworkFromString(serverConfigurationPointer, sourcesPathPointer, ref exception);
				}
			}

			if (exception != null)
			{
				throw new WebFrameworkException(exception);
			}
		}

		unsafe WebFramework(Config config)
		{
			void* exception = null;

			implementation = createWebFrameworkFromConfig(config.implementation, ref exception);

			if (exception != null)
			{
				throw new WebFrameworkException(exception);
			}
		}

		public delegate void OnStartServer();

		unsafe void Start(bool wait = false, OnStartServer? callback = null)
		{
			void* exception = null;

			startWebFrameworkServer(implementation, wait, callback == null ? IntPtr.Zero : Marshal.GetFunctionPointerForDelegate(callback), ref exception);

			if (exception != null)
			{
				throw new WebFrameworkException(exception);
			}
		}

		unsafe void Stop(bool wait = true)
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
			free(implementation);
		}
	}
}
