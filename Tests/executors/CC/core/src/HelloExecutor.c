#include <executors/executor.h>

#include <json_parser.h>

typedef struct
{
	int64_t number;
} HelloExecutor;

DEFINE_EXECUTOR(HelloExecutor, STATELESS_EXECUTOR);

static void service(executor_t executor, http_response_t response);

DEFINE_EXECUTOR_INIT(HelloExecutor)
{
	HelloExecutor* self = (HelloExecutor*)executor;
	json_parser_t parser = NULL;

	wf_get_executor_init_parameters(settings, &parser);

	wf_get_json_parser_integer(parser, "number", true, &self->number);
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

void service(executor_t executor, http_response_t response)
{
	HelloExecutor* self = (HelloExecutor*)executor;
	json_builder_t builder;

	wf_create_json_builder(&builder);

	wf_append_json_builder_string(builder, "message", "Hello, World!");
	wf_append_json_builder_integer(builder, "number", self->number);

	wf_set_json_body(response, builder);

	wf_delete_web_framework_json_builder(builder);
}
