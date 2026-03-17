#include <executors/executor.h>

DEFINE_DEFAULT_EXECUTOR(CCHelloExecutor, STATELESS_EXECUTOR)

DEFINE_EXECUTOR_METHOD(CCHelloExecutor, GET_METHOD, request, response)
{
	json_builder_t builder;

	web_framework_exception_t exception = wf_create_json_builder(&builder);

	if (exception != NULL)
	{
		const char* errorMessage = "Can't create json_builder_t";

		wf_set_http_response_code(response, INTERNAL_SERVER_ERROR);

		wf_set_body(response, errorMessage, strlen(errorMessage));
		
		return;
	}

	exception = wf_append_json_builder_string(builder, "message", "Hello from C");

	if (exception != NULL)
	{
		const char* errorMessage = "Can't append JSON string";

		wf_set_http_response_code(response, INTERNAL_SERVER_ERROR);

		wf_set_body(response, errorMessage, strlen(errorMessage));

		return;
	}

	wf_set_json_body(response, builder);

	wf_delete_json_builder(builder);
	wf_delete_web_framework_exception(exception);
}

DEFINE_INITIALIZE_WEB_FRAMEWORK()
