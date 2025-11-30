using Framework;
using Framework.Utility;

public class UploadMultipartExecutor : HeavyOperationStatelessExecutor
{
	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		List<Multipart> multiparts = [.. request.GetMultiparts()];

		foreach (Multipart multipart in multiparts)
		{
			using FileStream stream = new(multipart.Name, FileMode.Create);
			using BinaryWriter writer = new(stream);

			writer.Write(multipart.Data);
		}

		response.SetResponseCode(ResponseCodes.Created);
		response.SetBody("Finish uploading file");
	}
}
