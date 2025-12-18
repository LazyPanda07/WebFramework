#include <executors/executor.h>

typedef struct
{
	FILE* stream;
} UploadOctetStreamExecutor;

DEFINE_EXECUTOR(UploadOctetStreamExecutor, STATEFUL_EXECUTOR);

DEFINE_EXECUTOR_INIT(UploadOctetStreamExecutor)
{
	UploadOctetStreamExecutor* self = (UploadOctetStreamExecutor*)executor;

	self->stream = NULL;
}

DEFINE_EXECUTOR_METHOD(UploadOctetStreamExecutor, POST_METHOD, request, response)
{
	UploadOctetStreamExecutor* self = (UploadOctetStreamExecutor*)executor;
	char* endPtr = NULL;
	large_data_t* data = NULL;

	wf_get_large_data(request, &data);

	if (!self->stream)
	{
		const char* fileName;

		wf_get_http_header(request, "File-Name", &fileName);

		self->stream = fopen(fileName, "wb");
	}

	fwrite(data->data_part, sizeof(char), data->data_part_size, self->stream);

	if (data->is_last_packet)
	{
		fclose(self->stream);

		self->stream = NULL;

		wf_set_http_response_code(response, CREATED);
		wf_set_body(response, "Finish uploading file");
	}
}
