namespace Framework.Exceptions;

using System.Runtime.InteropServices;

using Framework.Utility;

/// <summary>
/// Exception class for WebFramework exceptions
/// </summary>
/// <param name="implementation"></param>
public sealed partial class WebFrameworkException(IntPtr implementation) : Exception, IDisposable
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial string getErrorMessage(IntPtr implementation);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteWebFrameworkException(IntPtr implementation);

	public void Dispose() => deleteWebFrameworkException(implementation);

	public override string Message
	{
		get => getErrorMessage(implementation);
	}
}
