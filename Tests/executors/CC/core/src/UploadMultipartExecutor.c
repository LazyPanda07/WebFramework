#include <APIExecutors/BaseExecutor.h>

DECLARE_DEFAULT_EXECUTOR(UploadMultipartExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR)

DECLARE_EXECUTOR_METHOD(UploadMultipartExecutor, POST_METHOD, request, response)
{
	Multipart* multiparts = NULL;
	size_t size;

	getMultiparts(request, &multiparts, &size);

	for (size_t i = 0; i < size; i++)
	{
		FILE* file = fopen(multiparts[i].fileName, "wb");

		fwrite(multiparts[i].data, sizeof(char), strlen(multiparts[i].data), file);

		fclose(file);
	}

	setHTTPResponseCode(response, CREATED);
	setBody(response, "Finish uploading files");

	free(multiparts);
}
