namespace Framework.Exceptions;

using System.Runtime.InteropServices;

using Framework.Utility;

/// <summary>
/// Exception class for WebFramework exceptions
/// </summary>
/// <param name="implementation"></param>
public sealed unsafe partial class WebFrameworkException(void* implementation) : Exception, IDisposable
{
	private readonly unsafe void* implementation = implementation;

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial char* getErrorMessage(void* implementation);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkObject(void* implementation);

	public void Dispose() => deleteWebFrameworkObject(implementation);

	public override string Message
	{
		get => Marshal.PtrToStringUTF8((IntPtr)getErrorMessage(implementation))!;
	}
}
