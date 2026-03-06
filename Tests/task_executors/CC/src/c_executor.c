#include <executors/executor.h>

#include <task_broker/task_brokers.h>
#include <task_broker/task_apis.h>

typedef struct message_struct
{
	const char* file_name;
	const char* message;
} message_struct_t;

static void serialize(void* taskStruct, json_object_t* arguments);

DEFINE_DEFAULT_EXECUTOR(c_executor, STATELESS_EXECUTOR)

DEFINE_EXECUTOR_METHOD(c_executor, GET_METHOD, request, response)
{
	message_struct_t taskStruct = { .file_name = "cc_cxx.txt", .message = "CC message" };

	wf_enqueue_task(request, INTERNAL_TASK_BROKER_NAME, CXX_TASK_BROKER_API, "CXXTestTaskExecutor", &taskStruct, serialize);

	taskStruct.file_name = "cc_cc.txt";

	wf_enqueue_task(request, INTERNAL_TASK_BROKER_NAME, CC_TASK_BROKER_API, "c_test_task_executor", &taskStruct, serialize);

	taskStruct.file_name = "cc_python.txt";

	wf_enqueue_task(request, INTERNAL_TASK_BROKER_NAME, PYTHON_TASK_BROKER_API, "PythonTestTaskExecutor", &taskStruct, serialize);

	taskStruct.file_name = "cc_csharp.txt";

	wf_enqueue_task(request, INTERNAL_TASK_BROKER_NAME, CSHARP_TASK_BROKER_API, "CSharpTestTaskExecutor", &taskStruct, serialize);

	wf_set_body(response, "Accepted");
	wf_set_http_response_code(response, ACCEPTED);
}

void serialize(void* taskStruct, json_object_t* arguments)
{
	message_struct_t* data = (message_struct_t*)taskStruct;
	json_object_t message;
	json_object_t file_name;

	wf_assign_json_object(arguments, "message", &message);
	wf_assign_json_object(arguments, "fileName", &file_name);

	wf_set_json_object_string(&message, data->message);
	wf_set_json_object_string(&file_name, data->file_name);
}
