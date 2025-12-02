using System.Collections.Immutable;

namespace Framework.Utility;

public static class WebFrameworkUtility
{
	public static ImmutableArray<SqlValue> MakeSqlValues(params object[] args)
	{
		SqlValue[] array = new SqlValue[args.Length];

		for (int i = 0; i < array.Length; i++)
		{
			array[i] = new(args[i]);
		}

		return ImmutableArray.Create(array);
	}
}
