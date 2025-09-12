#include <Executors/BaseExecutor.h>

DEFINE_DEFAULT_EXECUTOR(UploadMultipartExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR)

DEFINE_EXECUTOR_METHOD(UploadMultipartExecutor, POST_METHOD, request, response)
{
	Multipart_t* multiparts = NULL;
	size_t size;

	getMultiparts(request, &multiparts, &size);

	for (size_t i = 0; i < size; i++)
	{
		FILE* file = fopen(multiparts[i].name, "wb");

		fwrite(multiparts[i].data, sizeof(char), strlen(multiparts[i].data), file);

		fclose(file);
	}

	setHTTPResponseCode(response, CREATED);
	setBody(response, "Finish uploading files");

	free(multiparts);
}
