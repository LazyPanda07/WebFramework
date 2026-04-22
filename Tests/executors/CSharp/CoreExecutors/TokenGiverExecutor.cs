using Framework;
using Framework.Exceptions;
using Framework.Utility;
using System.Text.Json.Nodes;

public class TokenGiverExecutor : StatelessExecutor
{
	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		JsonObject temp = [];

		temp["userName"] = request.GetJson()["userName"].GetString();

		string token = WebFrameworkUtility.CreateJwt(temp, TimeSpan.FromMinutes(60));

		if (token != WebFrameworkUtility.CreateJwt(temp, TimeSpan.FromMinutes(60), request.GetWebFrameworkInstance()))
		{
			throw new WebFrameworkApiException("Failed to generate equal tokens", ResponseCodes.InternalServerError);
		}

		response.SetBody
		(
			new
			{
				token
			}
		);
	}
}
