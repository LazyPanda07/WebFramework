using System.Runtime.InteropServices;

namespace Framework
{
	public partial class Config
	{
		public unsafe readonly void* implementation;

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void* createConfigFromPath(char* configPath, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void* createConfigFromString(char* serverConfiguration, char* sourcesPath, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void overrideConfigurationString(char* key, char* value, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void overrideConfigurationInteger(char* key, long value, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void overrideConfigurationBoolean(char* key, [MarshalAs(UnmanagedType.Bool)] bool value, [MarshalAs(UnmanagedType.Bool)] bool recursive, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void overrideConfigurationStringArray(char* key, char** value, [MarshalAs(UnmanagedType.Bool)] bool recursive, long size, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void overrideConfigurationIntegerArray(char* key, long** value, [MarshalAs(UnmanagedType.Bool)] bool recursive, long size, ref void* exception);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void free(void* implementation);

		unsafe Config(string configPath)
		{
			if (!Path.Exists(configPath))
			{
				throw new Exception($"Path {configPath} doesn't exist");
			}

			void* exception = null;

			fixed (char* configPathPointer = configPath)
			{
				implementation = createConfigFromPath(configPathPointer, ref exception);
			}

			if (exception != null)
			{
				throw new WebFrameworkException(exception);
			}
		}

		unsafe Config(string serverConfiguration, string sourcesPath)
		{
			void* exception = null;

			fixed (char* serverConfigurationPointer = serverConfiguration)
			{
				fixed (char* sourcesPathPointer = sourcesPath)
				{
					implementation = createConfigFromString(serverConfigurationPointer, sourcesPathPointer, ref exception);
				}
			}

			if (exception != null)
			{
				throw new WebFrameworkException(exception);
			}
		}

		unsafe Config OverrideConfiguration(string key, string value, bool recursive = false)
		{
			void* exception = null;

			fixed (char* keyPointer = key)
			{
				fixed (char* valuePointer = value)
				{
					overrideConfigurationString(keyPointer, valuePointer, recursive, ref exception);
				}
			}

			if (exception != null)
			{
				throw new WebFrameworkException(exception);
			}

			return this;
		}

		unsafe Config OverrideConfiguration(string key, long value, bool recursive = false)
		{
			void* exception = null;

			fixed (char* keyPointer = key)
			{
				overrideConfigurationInteger(keyPointer, value, recursive, ref exception);
			}

			if (exception != null)
			{
				throw new WebFrameworkException(exception);
			}

			return this;
		}

		unsafe Config OverrideConfiguration(string key, bool value, bool recursive = false)
		{
			void* exception = null;

			fixed (char* keyPointer = key)
			{
				overrideConfigurationBoolean(keyPointer, value, recursive, ref exception);
			}

			if (exception != null)
			{
				throw new WebFrameworkException(exception);
			}

			return this;
		}

		unsafe Config OverrideConfiguration(string key, List<string> value, bool recursive = false)
		{
			//void* exception = null;

			//fixed (char* keyPointer = key)
			//{
			//	overrideConfigurationStringArray(keyPointer, value, recursive, (long)value.Count, ref exception);
			//}

			//if (exception != null)
			//{
			//	throw new WebFrameworkException(exception);
			//}

			return this;
		}

		unsafe Config OverrideConfiguration(string key, List<long> value, bool recursive = false)
		{
			//void* exception = null;

			//fixed (char* keyPointer = key)
			//{
			//	overrideConfigurationIntegerArray(keyPointer, value, recursive, (long)value.Count, ref exception);
			//}

			//if (exception != null)
			//{
			//	throw new WebFrameworkException(exception);
			//}

			return this;
		}

		unsafe ~Config()
		{
			free(implementation);
		}
	}
}
