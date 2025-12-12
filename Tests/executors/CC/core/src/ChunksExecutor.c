#include <executors/executor.h>

#include <signal.h>

#define SMALL_SIZE 15

typedef struct text_generator
{
	char* data;
	char* currentChunk;
	size_t offset;
} text_generator_t;

static const char* generate(void* data, size_t* size);

DEFINE_DEFAULT_EXECUTOR(ChunksExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(ChunksExecutor, GET_METHOD, request, response)
{
	text_generator_t generator =
	{
		.data = "Some information here",
		.currentChunk = NULL,
		.offset = 0
	};

	wf_send_chunks(request, response, generate, &generator);
}

DEFINE_EXECUTOR_METHOD(ChunksExecutor, POST_METHOD, request, response)
{
	text_generator_t generator =
	{
		.data = "Some information here",
		.currentChunk = NULL,
		.offset = 0
	};

	wf_send_file_chunks(request, response, "file.txt", generate, &generator);
}

const char* generate(void* data, size_t* size)
{
	text_generator_t* generator = (text_generator_t*)data;

	free(generator->currentChunk);

	if (generator->offset >= strlen(generator->data))
	{
		*size = 0;

		return NULL;
	}

	generator->currentChunk = (char*)calloc(SMALL_SIZE + 1, sizeof(char));

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

	*size = copySize;

	return generator->currentChunk;
}
