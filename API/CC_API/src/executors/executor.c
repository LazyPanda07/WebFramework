#include "executors/executor.h"

web_framework_exception_t wf_get_executor_init_parameters(executor_settings_t implementation, json_parser_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getExecutorInitParameters)(void* implementation, void** exception);

	web_framework_string_t temp = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorInitParameters, &exception);

	if (exception)
	{
		return exception;
	}

	exception = wf_create_json_parser_from_string(wf_get_data_from_string(temp), result);

	wf_delete_web_framework_string(temp);

	return exception;
}

web_framework_exception_t wf_get_executor_name(executor_settings_t implementation, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getExecutorName)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorName, &exception);

	return exception;
}

web_framework_exception_t wf_get_executor_user_agent_filter(executor_settings_t implementation, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getExecutorUserAgentFilter)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorUserAgentFilter, &exception);

	return exception;
}

web_framework_exception_t wf_get_executor_api_type(executor_settings_t implementation, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getExecutorAPIType)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorAPIType, &exception);

	return exception;
}

web_framework_exception_t wf_get_executor_load_type(executor_settings_t implementation, load_type_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef int (*getExecutorLoadType)(void* implementation, void** exception);

	*result = (load_type_t)CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorLoadType, &exception);

	return exception;
}
