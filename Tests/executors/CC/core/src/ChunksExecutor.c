#include <APIExecutors/BaseExecutor.h>

#define SMALL_SIZE 15

typedef struct
{
	char* data;
	const char* currentChunk;
	size_t offset;
} TextGenerator;

static const char* generate(void* data)
{
	TextGenerator* generator = (TextGenerator*)data;

	if (generator->offset >= strlen(generator->data))
	{
		return "";
	}

	generator->currentChunk = generator->data + generator->offset;
	generator->offset += SMALL_SIZE;

	return generator->currentChunk;
}

DECLARE_DEFAULT_EXECUTOR(ChunksExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR);

DECLARE_EXECUTOR_METHOD(ChunksExecutor, GET_METHOD, request, response)
{
	TextGenerator generator =
	{
		.data = "Some information here",
		.currentChunk = NULL,
		.offset = 0
	};

	sendChunks(request, response, generate, &generator);
}

DECLARE_EXECUTOR_METHOD(ChunksExecutor, POST_METHOD, request, response)
{
	TextGenerator generator =
	{
		.data = "Some information here",
		.currentChunk = NULL,
		.offset = 0
	};

	sendFileChunks(request, response, "file.txt", generate, &generator);
}
