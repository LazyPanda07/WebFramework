#include <executors/executor.h>

#include <utility/web_framework_utility.h>

DEFINE_DEFAULT_EXECUTOR(TokenGiverExecutor, STATELESS_EXECUTOR)

DEFINE_EXECUTOR_METHOD(TokenGiverExecutor, POST_METHOD, request, response)
{
	json_parser_t parser;
	json_object_t data;
	json_builder_t result;
	const char* user_name;
	web_framework_string_t token;
	web_framework_string_t second_token;
	web_framework_t instance;

	wf_get_request_json(request, &parser);
	wf_create_json_object(&data);
	wf_create_json_builder(&result);
	wf_get_web_framework_instance(request, &instance);

	wf_get_json_parser_string(parser, "userName", false, &user_name);

	{
		json_object_t temp;

		wf_assign_or_get_json_object(&data, "userName", &temp);

		wf_set_json_object_string(&temp, user_name);
	}

	wf_create_jwt_with_string(&data, 60, NULL, &token);
	wf_create_jwt_with_context(&data, 60, instance, &second_token);

	if (strcmp(wf_get_data_from_string(token), wf_get_data_from_string(second_token)))
	{
		const size_t exceptionHash = 0x3BC5B7CCF25A3A69;

		wf_throw_web_framework_exception(request, "Failed to generate equal tokens", INTERNAL_SERVER_ERROR, NULL, exceptionHash);
	}

	wf_append_json_builder_string(result, "token", wf_get_data_from_string(token));

	wf_set_json_body(response, result);

	wf_delete_json_object(&data);
	wf_delete_json_builder(result);
	wf_delete_string(token);
	wf_delete_string(second_token);
}
