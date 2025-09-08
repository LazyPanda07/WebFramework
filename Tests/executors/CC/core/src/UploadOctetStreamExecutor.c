#include <APIExecutors/BaseExecutor.h>

typedef struct
{
	FILE* stream;
	int64_t currentSize;
} UploadOctetStreamExecutor;

DEFINE_EXECUTOR(UploadOctetStreamExecutor, STATEFUL_EXECUTOR);

DEFINE_EXECUTOR_INIT(UploadOctetStreamExecutor)
{
	UploadOctetStreamExecutor* self = (UploadOctetStreamExecutor*)executor;

	self->stream = NULL;
	self->currentSize = 0;
}

DEFINE_EXECUTOR_METHOD(UploadOctetStreamExecutor, POST_METHOD, request, response)
{
	UploadOctetStreamExecutor* self = (UploadOctetStreamExecutor*)executor;
	const char* contentLength = NULL;
	char* endPtr = NULL;
	LargeData_t* data = NULL;

	getHTTPHeader(request, "Content-Length", &contentLength);
	getLargeData(request, &data);

	if (!self->stream)
	{
		const char* fileName;

		getHTTPHeader(request, "File-Name", &fileName);

		self->stream = fopen(fileName, "wb");
	}

	fwrite(data->dataPart, sizeof(char), data->dataPartSize, self->stream);

	self->currentSize += data->dataPartSize;

	if (self->currentSize == strtoll(contentLength, &endPtr, 10))
	{
		fclose(fopen("finish_uploading.txt", "w"));
	}

	if (data->isLastPacket)
	{
		fclose(self->stream);

		self->stream = NULL;

		setHTTPResponseCode(response, CREATED);
		setBody(response, "Finish uploading file");
	}
}
