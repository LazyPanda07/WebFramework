namespace Framework.Utility;

using System.Runtime.InteropServices;
using System.Text.Json.Nodes;
using Framework.Exceptions;

public static partial class WebFrameworkUtility
{
	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteWebFrameworkString(IntPtr implementation);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getDataFromString(IntPtr implementation);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr createJsonParserFromString(string jsonData, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr getJsonParserParsedData(IntPtr implementation, [MarshalAs(UnmanagedType.Bool)] bool weak, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial void deleteWebFrameworkJsonParser(IntPtr implementation);

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void ProgressCallback(float progress, [MarshalAs(UnmanagedType.LPUTF8Str)] string assetPath, IntPtr data);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void generateBinaryAssetFile(string directoryPath, string outputPath, ProgressCallback progressCallback, IntPtr data, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial IntPtr createJwtWithString(IntPtr data, long expirationTimeInMinutes, string jwtSecretVariableName, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr createJwtWithContext(IntPtr data, long expirationTimeInMinutes, IntPtr frameworkInstance, ref IntPtr exception);

	private static string GetStringData(IntPtr stringImplementation)
	{
		string result = Marshal.PtrToStringUTF8(getDataFromString(stringImplementation))!;

		deleteWebFrameworkString(stringImplementation);

		return result;
	}

	public static IList<SqlValue> MakeSqlValues(params object[] args)
	{
		SqlValue[] array = new SqlValue[args.Length];

		for (int i = 0; i < array.Length; i++)
		{
			array[i] = new(args[i]);
		}

		return array;
	}

	public static void GenerateBinaryAssetFile(string directoryPath, string outputPath, Action<float, string, object?>? progressCallback = null, object? data = null)
	{
		IntPtr exception = IntPtr.Zero;
		GCHandle? handle = null;

		if (data != null)
		{
			handle = GCHandle.Alloc(data);
		}

		generateBinaryAssetFile
		(
			directoryPath,
			outputPath,
			(float progress, string assetPath, IntPtr data) =>
			{
				if (progressCallback != null)
				{
					object? temp = null;

					if (data != IntPtr.Zero)
					{
						temp = GCHandle.FromIntPtr(data).Target;
					}

					progressCallback(progress, assetPath, temp);
				}
			},
			handle != null ? GCHandle.ToIntPtr((GCHandle)handle) : IntPtr.Zero,
			ref exception
		);

		handle?.Free();

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}
	}

	public static string CreateJwt(JsonObject data, TimeSpan expirationTime, string jwtSecretVariableName = "JWT_SECRET")
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr jsonParser = createJsonParserFromString(data.ToJsonString(), ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		IntPtr jsonData = getJsonParserParsedData(jsonParser, true, ref exception);

		if (exception != IntPtr.Zero)
		{
			deleteWebFrameworkJsonParser(jsonParser);

			throw new WebFrameworkException(exception);
		}

		IntPtr result = createJwtWithString(jsonData, (long)expirationTime.TotalMinutes, jwtSecretVariableName, ref exception);

		if (exception != IntPtr.Zero)
		{
			deleteWebFrameworkJsonParser(jsonParser);

			throw new WebFrameworkException(exception);
		}

		deleteWebFrameworkJsonParser(jsonParser);

		return GetStringData(result);
	}

	public static string CreateJwt(JsonObject data, TimeSpan expirationTime, WebFramework frameworkInstance)
	{
		IntPtr exception = IntPtr.Zero;
		IntPtr jsonParser = createJsonParserFromString(data.ToJsonString(), ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		IntPtr jsonData = getJsonParserParsedData(jsonParser, true, ref exception);

		if (exception != IntPtr.Zero)
		{
			deleteWebFrameworkJsonParser(jsonParser);

			throw new WebFrameworkException(exception);
		}

		IntPtr result = createJwtWithContext(jsonData, (long)expirationTime.TotalMinutes, frameworkInstance.implementation, ref exception);

		if (exception != IntPtr.Zero)
		{
			deleteWebFrameworkJsonParser(jsonParser);

			throw new WebFrameworkException(exception);
		}

		deleteWebFrameworkJsonParser(jsonParser);

		return GetStringData(result);
	}
}
