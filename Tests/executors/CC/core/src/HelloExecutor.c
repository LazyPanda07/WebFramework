#include <APIExecutors/BaseExecutor.h>
#include <JSONParser.h>

typedef struct
{
	int64_t number;
} HelloExecutor;

DECLARE_EXECUTOR(HelloExecutor, STATELESS_EXECUTOR);

DECLARE_EXECUTOR_INIT(HelloExecutor)
{
	HelloExecutor* self = (HelloExecutor*)executor;
	JSONParser parser = NULL;

	getExecutorInitParameters(settings, &parser);

	getJSONParserInteger(parser, "number", true, &self->number);
}

DECLARE_EXECUTOR_METHOD(HelloExecutor, GET_METHOD, request, response)
{
	HelloExecutor* self = (HelloExecutor*)executor;
	JSONBuilder builder;
	
	createJSONBuilder(&builder);

	appendJSONBuilderString(builder, "message", "Hello, World!");
	appendJSONBuilderInteger(builder, "number", self->number);

	setJSONBody(response, builder);

	deleteWebFrameworkJSONBuider(builder);
}
