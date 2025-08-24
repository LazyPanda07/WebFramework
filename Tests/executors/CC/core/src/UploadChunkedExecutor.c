#include <APIExecutors/BaseExecutor.h>

DECLARE_DEFAULT_EXECUTOR(UploadChunkedExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR);

DECLARE_EXECUTOR_METHOD(UploadChunkedExecutor, POST_METHOD, request, response)
{
	const char* fileName;
	HTTPChunk* chunks;
	size_t size;

	getHTTPHeader(request, "File-Name", &fileName);
	getHTTPChunks(request, &chunks, &size);

	FILE* file = fopen(fileName, "wb");

	for (size_t i = 0; i < size; i++)
	{
		fwrite(chunks[i].data, sizeof(char), chunks[i].size, file);
	}

	setHTTPResponseCode(response, CREATED);
	setBody(response, "Finish uploading file");

	free(chunks);
	fclose(file);
}
