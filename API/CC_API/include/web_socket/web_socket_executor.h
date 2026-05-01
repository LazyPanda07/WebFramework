#pragma once

#include "dll_handler.h"

typedef void* web_socket_executor_t;
typedef void* web_socket_frame_t;

typedef enum web_socket_frame_type
{
	FRAME_TYPE_TEXT = 0x1,
	FRAME_TYPE_BINARY = 0x2
} web_socket_frame_type_t;

/**
* Create custom executor_t function
* Used for loading function that creates executor_t instance
* @param structName Already defined struct name
*/
#define DEFINE_WEB_SOCKET_EXECUTOR(structName) WEB_FRAMEWORK_FUNCTIONS_API void* create##structName##WebSocketCCInstance()	\
{	\
	return malloc(sizeof(structName));	\
}	\
	\
WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCDeleteWebSocketExecutor##structName(void* executor)	\
{	\
	free((structName*)executor);	\
}

/**
* Create custom web_socket_executor_t function
* Used for loading function that creates web_socket_executor_t subclass
* @param structName Create empty struct for stateless executors
*/
#define DEFINE_DEFAULT_WEB_SOCKET_EXECUTOR(structName) typedef struct { char _; } structName; DEFINE_WEB_SOCKET_EXECUTOR(structName)

/**
 * Create on receive function
 * @param structName web_socket_executor_t name
 */
#define DEFINE_WEB_SOCKET_EXECUTOR_ON_RECEIVE(structName) WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCWebSocketExecutorOnReceive##structName(web_socket_executor_t executor, web_socket_frame_t frame, void(*send_data)(const uint8_t* data, uint64_t size, int32_t type))

/**
 * @brief Get payload from WebSocket frame
 * @param implementation web_socket_frame_t instance
 * @param payload Address of pointer
 * @param payload_size Size of payload
 * @return Error if occurred
 */
web_framework_exception_t wf_web_socket_frame_get_payload(web_socket_frame_t implementation, char** payload, uint64_t* payload_size);

/**
 * @brief Get WebSocket frame type
 * @param implementation web_socket_frame_t instance
 * @param type Value from web_socket_frame_type_t enum
 * @return Error if occurred
 */
web_framework_exception_t wf_web_socket_frame_get_type(web_socket_frame_t implementation, web_socket_frame_type_t* type);
