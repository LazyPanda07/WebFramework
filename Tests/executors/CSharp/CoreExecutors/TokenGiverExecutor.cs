using Framework;
using Framework.Utility;
using System.Text.Json.Nodes;

public class TokenGiverExecutor : StatelessExecutor
{
	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		JsonObject temp = [];

		temp["userName"] = request.GetJson()["userName"].GetString();

		response.SetBody
		(
			new
			{
				token = WebFrameworkUtility.CreateJwt(temp, TimeSpan.FromMinutes(60))
			}
		);
	}
}
