#include <executors/executor.h>

DEFINE_DEFAULT_EXECUTOR(TokenExecutor, STATELESS_EXECUTOR)

DEFINE_EXECUTOR_METHOD(TokenExecutor, GET_METHOD, request, response)
{
	json_object_t payload;
	const char* user_name;

	wf_get_token_payload(request, &payload);

	{
		json_object_t temp;

		wf_assign_or_get_json_object(&payload, "userName", &temp);

		wf_get_json_object_string(&temp, &user_name);
	}

	wf_set_body(response, user_name, strlen(user_name));

	wf_delete_json_object(&payload);
}
