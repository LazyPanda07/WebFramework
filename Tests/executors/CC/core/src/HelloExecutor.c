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
	JsonParser parser = NULL;

	getExecutorInitParameters(settings, &parser);

	getJsonParserInteger(parser, "number", true, &self->number);
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
	JsonBuilder builder;

	createJsonBuilder(&builder);

	appendJsonBuilderString(builder, "message", "Hello, World!");
	appendJsonBuilderInteger(builder, "number", self->number);

	setJsonBody(response, builder);

	deleteWebFrameworkJsonBuilder(builder);
}
