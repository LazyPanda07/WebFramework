using Framework.Utility;
using System.Text;

namespace CoreExecutors
{
	public class TextGenerator(string data) : ChunkGenerator
	{
		private readonly string data = data;
		private int offset = 0;

		public override byte[] Generate(ref bool finished)
		{
			const int smallStringSize = 15;

			if (offset >= data.Length)
			{
				finished = true;

				return [];
			}

			string temp = data.Substring(offset, Math.Min(smallStringSize, data.Length - offset));

			offset += temp.Length;

			return Encoding.UTF8.GetBytes(temp);
		}
	}
}
