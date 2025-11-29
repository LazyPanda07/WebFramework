using Framework;
using Framework.Utility;

public class LocalizationExecutor : StatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		response.SetBody
		(
			new
			{
				result = WebFrameworkLocalization.GetLocalizedString("LocalizationData", "key", (string)request.GetJson()["language"])
			}
		);
	}
}
