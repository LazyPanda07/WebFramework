namespace Framework.Utility;

using System.Runtime.InteropServices;

public abstract class ChunkGenerator
{
	internal IntPtr currentBuffer = IntPtr.Zero;

	public abstract ReadOnlySpan<char> Generate(ref bool finished);

	~ChunkGenerator()
	{
		if (currentBuffer != IntPtr.Zero)
		{
			Marshal.FreeHGlobal(currentBuffer);
		}

		currentBuffer = IntPtr.Zero;
	}
}
