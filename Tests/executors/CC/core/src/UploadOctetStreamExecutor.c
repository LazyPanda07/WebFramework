#include <APIExecutors/BaseExecutor.h>

typedef struct
{
	FILE* stream;
} UploadOctetStreamExecutor;

DECLARE_EXECUTOR(UploadOctetStreamExecutor, STATEFUL_EXECUTOR);

DECLARE_EXECUTOR_INIT(UploadOctetStreamExecutor)
{
	UploadOctetStreamExecutor* self = (UploadOctetStreamExecutor*)executor;

	self->stream = NULL;
}

DECLARE_EXECUTOR_METHOD(UploadOctetStreamExecutor, POST_METHOD, request, response)
{
	UploadOctetStreamExecutor* self = (UploadOctetStreamExecutor*)executor;
	LargeData_t* data = NULL;

	getLargeData(request, &data);

	if (!self->stream)
	{
		const char* fileName;

		getHTTPHeader(request, "File-Name", &fileName);

		self->stream = fopen(fileName, "wb");
	}

	fwrite(data->dataPart, sizeof(char), data->dataPartSize, self->stream);

	if (data->isLastPacket)
	{
		fclose(self->stream);

		self->stream = NULL;

		setHTTPResponseCode(response, CREATED);
		setBody(response, "Finish uploading file");

		fclose(fopen("finish_uploading.txt", "w"));
	}
}
