#include <Executors/Executor.h>
#include <JsonParser.h>

typedef struct
{
	int64_t number;
} HelloExecutor;

DEFINE_EXECUTOR(HelloExecutor, STATELESS_EXECUTOR);

static void service(Executor executor, HTTPResponse response);

DEFINE_EXECUTOR_INIT(HelloExecutor)
{
	HelloExecutor* self = (HelloExecutor*)executor;
	JSONParser parser = NULL;

	getExecutorInitParameters(settings, &parser);

	getJSONParserInteger(parser, "number", true, &self->number);
}

DEFINE_EXECUTOR_METHOD(HelloExecutor, GET_METHOD, request, response)
{
	service(executor, response);
}

DEFINE_EXECUTOR_METHOD(HelloExecutor, POST_METHOD, request, response)
{
	service(executor, response);
}

DEFINE_EXECUTOR_METHOD(HelloExecutor, HEAD_METHOD, request, response)
{
	service(executor, response);
}

DEFINE_EXECUTOR_METHOD(HelloExecutor, PUT_METHOD, request, response)
{
	service(executor, response);
}

DEFINE_EXECUTOR_METHOD(HelloExecutor, DELETE_METHOD, request, response)
{
	service(executor, response);
}

DEFINE_EXECUTOR_METHOD(HelloExecutor, PATCH_METHOD, request, response)
{
	service(executor, response);
}

DEFINE_EXECUTOR_METHOD(HelloExecutor, CONNECT_METHOD, request, response)
{
	service(executor, response);
}

void service(Executor executor, HTTPResponse response)
{
	HelloExecutor* self = (HelloExecutor*)executor;
	JSONBuilder builder;

	createJSONBuilder(&builder);

	appendJSONBuilderString(builder, "message", "Hello, World!");
	appendJSONBuilderInteger(builder, "number", self->number);

	setJSONBody(response, builder);

	deleteWebFrameworkJSONBuilder(builder);
}
