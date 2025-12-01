using Framework.Utility;

namespace Framework.Exceptions;

public class NotImplementedDoMethodException(string methodName, Type type) : WebFrameworkApiException($"Not implemented method {methodName} in {type.Name}", ResponseCodes.BadRequest)
{

}
