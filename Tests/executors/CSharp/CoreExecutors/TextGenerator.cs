using Framework.Utility;
using System.Text;

namespace CoreExecutors
{
	public class TextGenerator(string data) : ChunkGenerator
	{
		private readonly string data = data;
		private int offset = 0;
		private byte[] lastResult = [];

		public override ReadOnlySpan<byte> Generate(ref bool finished)
		{
			const int smallStringSize = 15;

			if (offset >= data.Length)
			{
				finished = true;

				return new();
			}

			string temp = data.Substring(offset, Math.Min(smallStringSize, data.Length - offset));

			offset += temp.Length;

			lastResult = Encoding.UTF8.GetBytes(temp);

			return lastResult;
		}
	}
}
