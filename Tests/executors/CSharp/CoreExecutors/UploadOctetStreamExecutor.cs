using Framework;
using Framework.Utility;

public class UploadOctetStreamExecutor : StatefulExecutor
{
	private FileStream? stream;

	public override void DoPost(HttpRequest request, HttpResponse response)
	{
		var (data, last) = request.GetLargeData();

		stream ??= new(request.GetHeaders()["File-Name"], FileMode.Create);

		stream.Write(data);

		if (last)
		{
			stream.Dispose();

			stream = null;

			response.SetResponseCode(ResponseCodes.Created);
			response.SetBody("Finish uploading file");
		}
	}
}
