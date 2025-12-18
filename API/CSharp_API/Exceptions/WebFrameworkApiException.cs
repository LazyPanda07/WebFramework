namespace Framework.Exceptions;

using Framework.Utility;

public class WebFrameworkApiException(string errorMessage, ResponseCodes responceCode, string logCategory = "") : Exception(errorMessage)
{
	public string LogCategory { get; } = logCategory;
	public ResponseCodes ResponseCode { get; } = responceCode;
}
