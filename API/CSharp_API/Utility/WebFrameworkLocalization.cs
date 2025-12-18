using Framework.Exceptions;
using System.Runtime.InteropServices;

namespace Framework.Utility;

public static unsafe partial class WebFrameworkLocalization
{
	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr getWebFrameworkLocalizedString(string localizationModuleName, string key, string language, ref void* exception);

	public static string GetLocalizedString(string localizationModuleName, string key, string? language = null)
	{
		void* exception = null;
		string result = Marshal.PtrToStringUTF8(getWebFrameworkLocalizedString(localizationModuleName, key, language ?? "", ref exception))!;

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return result;
	}
}
