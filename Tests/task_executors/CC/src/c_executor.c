#include <executors/executor.h>

#include <task_broker/task_brokers.h>
#include <task_broker/task_apis.h>

#define FILE_NAME_SIZE 32

typedef struct message_struct
{
	char* file_name;
	const char* message;
} message_struct_t;

static void serialize(void* task_struct, json_object_t* arguments);

DEFINE_DEFAULT_EXECUTOR(c_executor, STATELESS_EXECUTOR)

DEFINE_EXECUTOR_METHOD(c_executor, GET_METHOD, request, response)
{
	json_parser_t parser;
	char* task_broker = NULL;
	const char* response_message = "Accepted";
	message_struct_t task_struct = { .file_name = NULL, .message = "CC message" };
	
	task_struct.file_name = calloc(FILE_NAME_SIZE, sizeof(char));

	wf_get_request_json(request, &parser);

	wf_get_json_parser_string(parser, "taskBroker", false, &task_broker);

	snprintf(task_struct.file_name, FILE_NAME_SIZE, "cc_cxx_%s.txt", task_broker);

	wf_enqueue_task(request, task_broker, CXX_TASK_BROKER_API, "CXXTestTaskExecutor", &task_struct, serialize);

	snprintf(task_struct.file_name, FILE_NAME_SIZE, "cc_cc_%s.txt", task_broker);

	wf_enqueue_task(request, task_broker, CC_TASK_BROKER_API, "c_test_task_executor", &task_struct, serialize);

	snprintf(task_struct.file_name, FILE_NAME_SIZE, "cc_python_%s.txt", task_broker);

	wf_enqueue_task(request, task_broker, PYTHON_TASK_BROKER_API, "PythonTestTaskExecutor", &task_struct, serialize);

	snprintf(task_struct.file_name, FILE_NAME_SIZE, "cc_csharp_%s.txt", task_broker);

	wf_enqueue_task(request, task_broker, CSHARP_TASK_BROKER_API, "CSharpTestTaskExecutor", &task_struct, serialize);

	wf_set_body(response, response_message, strlen(response_message));
	wf_set_http_response_code(response, ACCEPTED);

	free(task_struct.file_name);
}

void serialize(void* task_struct, json_object_t* arguments)
{
	message_struct_t* data = (message_struct_t*)task_struct;
	json_object_t message;
	json_object_t file_name;

	wf_assign_json_object(arguments, "message", &message);
	wf_assign_json_object(arguments, "fileName", &file_name);

	wf_set_json_object_string(&message, data->message);
	wf_set_json_object_string(&file_name, data->file_name);
}
