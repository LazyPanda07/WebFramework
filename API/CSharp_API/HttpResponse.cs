using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;

namespace Framework;

public sealed unsafe partial class HttpResponse(nint implementation)
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void setHTTPResponseBody(IntPtr implementation, string body, ref void* exception);

	public void SetBody(string body)
	{
		void* exception = null;

		setHTTPResponseBody(implementation, body, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}
}
