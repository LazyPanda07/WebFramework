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

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void setResponseBody(IntPtr implementation, byte[] body, nuint bodySize, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void setResponseJsonBody(IntPtr implementation, IntPtr jsonBuilder, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void setResponseCode(IntPtr implementation, ResponseCodes code, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void addResponseHeader(IntPtr implementation, string name, string value, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void appendResponseBody(IntPtr implementation, string body, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void addResponseCookie(IntPtr implementation, string name, string value, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void setResponseDefault(IntPtr implementation, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void setResponseIsValid(IntPtr implementation, [MarshalAs(UnmanagedType.Bool)] bool isValid, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr createJsonBuilderFromString(string jsonData, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteWebFrameworkJsonBuilder(IntPtr implementation);

	public void SetBody(string body)
	{
		IntPtr exception = IntPtr.Zero;
		byte[] data = Encoding.UTF8.GetBytes(body);

		setResponseBody(implementation, data, (nuint)data.Length, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetBody(byte[] body)
	{
		IntPtr exception = IntPtr.Zero;

		setResponseBody(implementation, body, (nuint)body.Length, ref exception);

		if (exception != IntPtr.Zero)
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

		IntPtr exception = IntPtr.Zero;
		string jsonData = JsonSerializer.Serialize(data, wrapperOptions);
		IntPtr jsonBuilder = createJsonBuilderFromString(jsonData, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		setResponseJsonBody(implementation, jsonBuilder, ref exception);

		deleteWebFrameworkJsonBuilder(jsonBuilder);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetResponseCode(ResponseCodes code)
	{
		IntPtr exception = IntPtr.Zero;

		setResponseCode(implementation, code, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void AddHeader(string name, string value)
	{
		IntPtr exception = IntPtr.Zero;

		addResponseHeader(implementation, name, value, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void AddCookie(string name, string value)
	{
		IntPtr exception = IntPtr.Zero;

		addResponseCookie(implementation, name, value, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public HttpResponse AppendBody(string body)
	{
		IntPtr exception = IntPtr.Zero;

		appendResponseBody(implementation, body, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return this;
	}

	public void SetDefault()
	{
		IntPtr exception = IntPtr.Zero;

		setResponseDefault(implementation, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public void SetIsValid(bool isValid)
	{
		IntPtr exception = IntPtr.Zero;

		setResponseIsValid(implementation, isValid, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}
	}
}
