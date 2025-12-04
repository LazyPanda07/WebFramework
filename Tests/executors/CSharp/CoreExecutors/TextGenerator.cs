using Framework.Utility;

namespace CoreExecutors
{
	public class TextGenerator(string data) : ChunkGenerator
	{
		private readonly string data = data;
		private int offset = 0;
		private string lastResult = "";

		public override ReadOnlySpan<char> Generate(ref bool finished)
		{
			const int smallStringSize = 15;

			if (offset >= data.Length)
			{
				finished = true;

				return new();
			}

			lastResult = data.Substring(offset, Math.Min(smallStringSize, data.Length - offset));

			offset += lastResult.Length;

			return lastResult;
		}
	}
}
