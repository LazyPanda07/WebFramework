#pragma once

#include "dll_handler.h"

typedef void* web_socket_executor_t;
typedef void* web_socket_frame_t;

typedef enum web_socket_frame_type
{
	FRAME_TYPE_CONTINUATION = 0x0,
	FRAME_TYPE_TEXT = 0x1,
	FRAME_TYPE_BINARY = 0x2,
	FRAME_TYPE_CLOSE = 0x8,
	FRAME_TYPE_PING = 0x9,
	FRAME_TYPE_PONG = 0xA
} web_socket_frame_type_t;

/**
 * Create custom executor_t function
 * Used for loading function that creates executor_t instance
 * @param struct_name Already defined struct name
*/
#define DEFINE_WEB_SOCKET_EXECUTOR(struct_name) WEB_FRAMEWORK_FUNCTIONS_API void* create##struct_name##WebSocketCCInstance()	\
{	\
	return malloc(sizeof(struct_name));	\
}	\
	\
WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCDeleteWebSocketExecutor##struct_name(void* executor)	\
{	\
	free((struct_name*)executor);	\
}

/**
 * Create custom web_socket_executor_t function
 * Used for loading function that creates web_socket_executor_t subclass
 * @param struct_name Create empty struct for stateless executors
*/
#define DEFINE_DEFAULT_WEB_SOCKET_EXECUTOR(struct_name) typedef struct { char _; } struct_name; DEFINE_WEB_SOCKET_EXECUTOR(struct_name)

/**
 * Create on receive function
 * @param struct_name web_socket_executor_t name
 */
#define DEFINE_WEB_SOCKET_EXECUTOR_ON_RECEIVE(struct_name) WEB_FRAMEWORK_FUNCTIONS_API void webFrameworkCCWebSocketExecutorOnReceive##struct_name(web_socket_executor_t executor, web_socket_frame_t frameImplementation, void(*send_data)(const uint8_t* data, uint64_t size, int32_t type, void* additionalData), void* additionalData)

#define WF_GET_WEB_SOCKET_EXECUTOR(struct_name) ((*struct_name)executor)

#define WF_GET_WEB_SOCKET_FRAME() ((web_socket_frame_t)frameImplementation)

 /**
  * Send WebSocket frame
  * @param data Payload in const char* or NULL
  * @param size Size in bytes of data
  * @param type web_socket_frame_type_t
 */
#define WF_SEND_WEB_SOCKET_FRAME(data, size, type) send_data((const uint8_t*)data, size, (int32_t)type, additionalData)

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
