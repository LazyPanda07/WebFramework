namespace Framework;

using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Encodings.Web;
using System.Text.Json;
using System.Text.Unicode;

public sealed unsafe partial class HttpResponse(nint implementation)
{
	internal readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setResponseBody(IntPtr implementation, byte[] body, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setResponseJSONBody(IntPtr implementation, IntPtr jsonBuilder, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setResponseCode(IntPtr implementation, ResponseCodes code, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void addResponseHeader(IntPtr implementation, string name, string value, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void appendResponseBody(IntPtr implementation, string body, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial void addResponseCookie(IntPtr implementation, string name, string value, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setResponseDefault(IntPtr implementation, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void setResponseIsValid(IntPtr implementation, [MarshalAs(UnmanagedType.Bool)] bool isValid, ref void* exception);

	[LibraryImport(DLLHandler.libraryName, StringMarshalling = StringMarshalling.Utf8)]
	private static unsafe partial IntPtr createJSONBuilderFromString(string jsonData, ref void* exception);

	[LibraryImport(DLLHandler.libraryName)]
	private static unsafe partial void deleteWebFrameworkJSONBuilder(IntPtr implementation);

	public void SetBody(string body)
	{
		void* exception = null;

		setResponseBody(implementation, Encoding.UTF8.GetBytes(body), ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetBody(byte[] body)
	{
		void* exception = null;

		setResponseBody(implementation, body, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetBody(object data, JsonSerializerOptions? options = null)
	{
		JsonSerializerOptions wrapperOptions;

		if (options != null)
		{
			wrapperOptions = new(options);
		}
		else
		{
			wrapperOptions = new();
		}

		wrapperOptions.Encoder = JavaScriptEncoder.Create(UnicodeRanges.All);
		wrapperOptions.WriteIndented = false;

		void* exception = null;
		string jsonData = JsonSerializer.Serialize(data, wrapperOptions);
		IntPtr jsonBuilder = createJSONBuilderFromString(jsonData, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		setResponseJSONBody(implementation, jsonBuilder, ref exception);

		deleteWebFrameworkJSONBuilder(jsonBuilder);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetResponseCode(ResponseCodes code)
	{
		void* exception = null;

		setResponseCode(implementation, code, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void AddHeader(string name, string value)
	{
		void* exeption = null;

		addResponseHeader(implementation, name, value, ref exeption);

		if (exeption != null)
		{
			throw new WebFrameworkException(exeption);
		}
	}

	public void AddCookie(string name, string value)
	{
		void* exeption = null;

		addResponseCookie(implementation, name, value, ref exeption);

		if (exeption != null)
		{
			throw new WebFrameworkException(exeption);
		}
	}

	public HttpResponse AppendBody(string body)
	{
		void* exception = null;

		appendResponseBody(implementation, body, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	public void SetDefault()
	{
		void* exception = null;

		setResponseDefault(implementation, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetIsValid(bool isValid)
	{
		void* exception = null;

		setResponseIsValid(implementation, isValid, ref exception);

		if (exception != null)
		{
			throw new WebFrameworkException(exception);
		}
	}
}
