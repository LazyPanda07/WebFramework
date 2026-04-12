namespace Framework.Utility;

using System.Runtime.InteropServices;
using Framework.Exceptions;

public static partial class WebFrameworkUtility
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	private delegate void ProgressCallback(float progress, [MarshalAs(UnmanagedType.LPUTF8Str)] string assetPath, IntPtr data);

	[LibraryImport(DLLHandler.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8)]
	private static partial void generateBinaryAssetFile(string directoryPath, string outputPath, ProgressCallback progressCallback, IntPtr data, ref IntPtr exception);

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
}
