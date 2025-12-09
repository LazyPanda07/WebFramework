#include "Executors/Executor.h"

WebFrameworkException getExecutorInitParameters(ExecutorSettings implementation, JSONParser* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getExecutorInitParameters)(void* implementation, void** exception);

	WebFrameworkString temp = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorInitParameters, &exception);

	if (exception)
	{
		return exception;
	}

	exception = createJSONParserFromString(getDataFromString(temp), result);

	deleteWebFrameworkString(temp);

	return exception;
}

WebFrameworkException getExecutorName(ExecutorSettings implementation, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getExecutorName)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorName, &exception);

	return exception;
}

WebFrameworkException getExecutorUserAgentFilter(ExecutorSettings implementation, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getExecutorUserAgentFilter)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorUserAgentFilter, &exception);

	return exception;
}

WebFrameworkException getExecutorAPIType(ExecutorSettings implementation, WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getExecutorAPIType)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorAPIType, &exception);

	return exception;
}

WebFrameworkException getExecutorLoadType(ExecutorSettings implementation, LoadType_t* result)
{
	WebFrameworkException exception = NULL;

	typedef int (*getExecutorLoadType)(void* implementation, void** exception);

	*result = (LoadType_t)CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getExecutorLoadType, &exception);

	return exception;
}
