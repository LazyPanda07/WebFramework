using Framework;
using Framework.Utility;

public class UploadChunkedExecutor : HeavyOperationStatelessExecutor
{
	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		using FileStream stream = new(request.GetHeaders()["File-Name"], FileMode.Create);
		using BinaryWriter writer = new(stream);
		List<byte[]> chunks = [.. request.GetChunks()];

		foreach(byte[] chunk in chunks)
		{
			writer.Write(chunk);
		}

		response.SetResponseCode(ResponseCodes.Created);
		response.SetBody("Finish uploading file");
	}
}
