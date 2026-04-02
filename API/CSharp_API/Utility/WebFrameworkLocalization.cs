using Framework.Exceptions;
using System.Runtime.InteropServices;

namespace Framework.Utility;

public static unsafe partial class WebFrameworkLocalization
{
	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getWebFrameworkLocalizedString(string localizationModuleName, string key, string language, ref IntPtr exception);

	public static string GetLocalizedString(string localizationModuleName, string key, string? language = null)
	{
		IntPtr exception = IntPtr.Zero;
		string result = Marshal.PtrToStringUTF8(getWebFrameworkLocalizedString(localizationModuleName, key, language ?? "", ref exception))!;

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}
}
