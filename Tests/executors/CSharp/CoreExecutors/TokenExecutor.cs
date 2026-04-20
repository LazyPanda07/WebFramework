using Framework;
using System.Text.Json.Nodes;

public class TokenExecutor : StatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		JsonObject data = request.GetTokenPayload()!;

		response.SetBody(data["userName"]!.GetValue<string>());
	}
}
