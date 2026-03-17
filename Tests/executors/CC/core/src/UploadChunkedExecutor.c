#include <executors/executor.h>

DEFINE_DEFAULT_EXECUTOR(UploadChunkedExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR)

DEFINE_EXECUTOR_METHOD(UploadChunkedExecutor, POST_METHOD, request, response)
{
	const char* response_message = "Finish uploading file";
	const char* fileName;
	http_chunk_t* chunks;
	size_t size;

	wf_get_http_header(request, "File-Name", &fileName);
	wf_get_chunks(request, &chunks, &size);

	FILE* file = fopen(fileName, "wb");

	for (size_t i = 0; i < size; i++)
	{
		fwrite(chunks[i].data, sizeof(char), chunks[i].size, file);
	}

	wf_set_http_response_code(response, CREATED);
	wf_set_body(response, response_message, strlen(response_message));

	free(chunks);
	fclose(file);
}
