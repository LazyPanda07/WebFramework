#include "web_socket/web_socket_executor.h"

web_framework_exception_t wf_web_socket_frame_get_payload(web_socket_frame_t implementation, char** payload, uint64_t* payload_size)
{
	web_framework_exception_t exception = NULL;
	
	typedef char* (*getFramePayload)(void* implementation, uint64_t* size, void** exception);

	*payload = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getFramePayload, &payload_size, &exception);

	return exception;
}

web_framework_exception_t wf_web_socket_frame_get_type(web_socket_frame_t implementation, web_socket_frame_type_t* type)
{
	web_framework_exception_t exception = NULL;

	typedef int32_t (*getFrameType)(void* implementation, void** exception);

	*type = (web_socket_frame_type_t)CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getFrameType, &exception);

	return exception;
}
