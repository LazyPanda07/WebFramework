#include <web_socket/web_socket_executor.h>

DEFINE_DEFAULT_WEB_SOCKET_EXECUTOR(WebSocketEcho);

DEFINE_WEB_SOCKET_EXECUTOR_ON_RECEIVE(WebSocketEcho)
{
	web_socket_frame_t frame = WF_GET_WEB_SOCKET_FRAME();
	char* payload;
	uint64_t size;
	web_socket_frame_type_t type;

	wf_web_socket_frame_get_payload(frame, &payload, &size);
	wf_web_socket_frame_get_type(frame, &type);

	WF_SEND_WEB_SOCKET_FRAME(payload, size, type);
}
