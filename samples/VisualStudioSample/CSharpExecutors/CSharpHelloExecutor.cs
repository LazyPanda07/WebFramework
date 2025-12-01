using Framework;
using Framework.Exceptions;

public class CSharpHelloExecutor : StatelessExecutor
{
	public override void DoGet(HttpRequest request, HttpResponse response)
	{
		response.SetBody
		(
			new
			{
				message = "Hello, World from C#!"
			}
		);
	}
}
