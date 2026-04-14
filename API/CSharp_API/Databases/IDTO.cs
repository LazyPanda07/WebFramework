namespace Framework.Databases
{
	public interface IDTO<T> where T : IDTO<T>
	{
		public static abstract T Create(IDictionary<string, SqlValue> row);
	}
}
