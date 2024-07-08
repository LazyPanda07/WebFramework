﻿using System.Runtime.InteropServices;

namespace Framework
{
	public unsafe partial class WebFrameworkException(void* implementation) : Exception
	{
		private readonly unsafe void* implementation = implementation;

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial char* getErrorMessage(void* implementation);

		[LibraryImport(DLLHandler.libraryName)]
		private static unsafe partial void free(void* implementation);

		public override string Message
		{
#pragma warning disable CS8603 // Possible null reference return.
			get => Marshal.PtrToStringUTF8((IntPtr)getErrorMessage(implementation));
#pragma warning restore CS8603 // Possible null reference return.
		}

		unsafe ~WebFrameworkException()
		{
			free(implementation);
		}
	}
}
