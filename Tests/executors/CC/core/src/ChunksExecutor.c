#include <APIExecutors/BaseExecutor.h>

#include <signal.h>

#define SMALL_SIZE 15

typedef struct
{
	char* data;
	char* currentChunk;
	size_t offset;
} TextGenerator;

static const char* generate(void* data)
{
	TextGenerator* generator = (TextGenerator*)data;

	free(generator->currentChunk);

	if (generator->offset >= strlen(generator->data))
	{
		return NULL;
	}

	generator->currentChunk = (char*)calloc(SMALL_SIZE, sizeof(char));

	if (!generator->currentChunk)
	{
		raise(SIGSEGV);

		return NULL;
	}

	size_t copySize = SMALL_SIZE;

	if (strlen(generator->data) < generator->offset + copySize)
	{
		copySize = strlen(generator->data) - generator->offset;
	}

	memcpy(generator->currentChunk, generator->data + generator->offset, copySize);

	generator->offset += copySize;

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
