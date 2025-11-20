namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json;

public sealed unsafe partial class HttpResponse(nint implementation)
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setHTTPResponseBody(IntPtr implementation, byte[] body, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setHTTPResponseJSONBody(IntPtr implementation, IntPtr jsonBuilder, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setHTTPResponseCode(IntPtr implementation, ResponseCodes code, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void addHTTPResponseHeader(IntPtr implementation, string name, string value, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void appendHTTPResponseBody(IntPtr implementation, string body, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void addHTTPResponseCookie(IntPtr implementation, string name, string value, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setHTTPResponseDefault(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setHTTPResponseIsValid(IntPtr implementation, [MarshalAs(UnmanagedType.Bool)] bool isValid, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr createJSONBuilderFromString(string jsonData, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkJSONBuilder(IntPtr implementation);

	public void SetBody(string body)
	{
		void* exception = null;

		setHTTPResponseBody(implementation, Encoding.UTF8.GetBytes(body), ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetBody(byte[] body)
	{
		void* exception = null;

		setHTTPResponseBody(implementation, body, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetBody(object data, JsonSerializerOptions? options = null)
	{
		void* exception = null;
		string jsonData = JsonSerializer.Serialize(data, options);
		IntPtr jsonBuilder = createJSONBuilderFromString(jsonData, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		setHTTPResponseJSONBody(implementation, jsonBuilder, ref exception);

		deleteWebFrameworkJSONBuilder(jsonBuilder);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetResponseCode(ResponseCodes code)
	{
		void* exception = null;

		setHTTPResponseCode(implementation, code, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void AddHeader(string name, string value)
	{
		void* exeption = null;

		addHTTPResponseHeader(implementation, name, value, ref exeption);

		if (exeption != null)
		{
			throw new WebFrameworkException(exeption);
		}
	}

	public void AddCookie(string name, string value)
	{
		void* exeption = null;

		addHTTPResponseCookie(implementation, name, value, ref exeption);

		if (exeption != null)
		{
			throw new WebFrameworkException(exeption);
		}
	}

	public HttpResponse AppendBody(string body)
	{
		void* exception = null;

		appendHTTPResponseBody(implementation, body, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	public void SetDefault()
	{
		void* exception = null;

		setHTTPResponseDefault(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetIsValid(bool isValid)
	{
		void* exception = null;

		setHTTPResponseIsValid(implementation, isValid, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}
}
