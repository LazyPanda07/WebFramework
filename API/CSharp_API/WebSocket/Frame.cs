using Framework.Exceptions;
using Framework.Utility;
using System.Runtime.InteropServices;

namespace Framework;

public abstract record FramePayload
{
	private FramePayload()
	{

	}

	public sealed record ContinuationFramePayload(string Payload) : FramePayload;
	public sealed record TextFramePayload(string Payload) : FramePayload;
	public sealed record BinaryFramePayload(IList<byte> Payload) : FramePayload;
	public sealed record CloseFramePayload(string Payload = "") : FramePayload;
	public sealed record PingFramePayload(string Payload = "") : FramePayload;
	public sealed record PongFramePayload(string Payload = "") : FramePayload;
}

public sealed partial class Frame(IntPtr implementation)
{
	private readonly IntPtr implementation = implementation;

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial IntPtr getFramePayload(IntPtr implementation, ref ulong size, ref IntPtr exception);

	[LibraryImport(DLLHandler.LIBRARY_NAME)]
	private static partial int getFrameType(IntPtr implementation, ref IntPtr exception);

	public enum Type
	{
		continuation = 0x0,
		text = 0x1,
		binary = 0x2,
		close = 0x8,
		ping = 0x9,
		pong = 0xA
	}

	public FramePayload GetPayload()
	{
		IntPtr exception = IntPtr.Zero;
		ulong size = 0;

		IntPtr result = getFramePayload(implementation, ref size, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		Type type = GetFrameType();

		switch (type)
		{
			case Type.continuation:
				return new FramePayload.ContinuationFramePayload(Marshal.PtrToStringUTF8(result, (int)size));

			case Type.text:
				return new FramePayload.TextFramePayload(Marshal.PtrToStringUTF8(result, (int)size));

			case Type.binary:
				byte[] data = new byte[size];

				Marshal.Copy(result, data, 0, (int)size);

				return new FramePayload.BinaryFramePayload(data);

			case Type.close:
				return new FramePayload.CloseFramePayload(Marshal.PtrToStringUTF8(result, (int)size));

			case Type.ping:
				return new FramePayload.PingFramePayload(Marshal.PtrToStringUTF8(result, (int)size));

			case Type.pong:
				return new FramePayload.PongFramePayload(Marshal.PtrToStringUTF8(result, (int)size));

			default:
				throw new Exception($"Wrong Frame.Type: {((int)type)}");
		}
	}

	public Type GetFrameType()
	{
		IntPtr exception = IntPtr.Zero;

		int result = getFrameType(implementation, ref exception);

		if (exception != IntPtr.Zero)
		{
			throw new WebFrameworkException(exception);
		}

		return (Type)result;
	}
}
