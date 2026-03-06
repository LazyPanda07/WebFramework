#include <task_broker/task_executor.h>

#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>

#include <json_parser.h>

DEFINE_DEFAULT_TASK_EXECUTOR(c_test_task_executor)
{
	json_object_t arguments = WF_GET_TASK_EXECUTOR_ARGUMENTS();
	FILE* file = NULL;

	fopen_s(&file, "c_task_executor.txt", "w");

	if (file)
	{
		json_parser_t parser;
		char* message = NULL;

		wf_create_json_parser_from_object(&parser, &arguments);

		wf_get_json_parser_string(parser, "message", false, &message);

		fwrite(message, sizeof(char), strlen(message), file);

		fclose(file);
	}
}

DEFINE_INITIALIZE_WEB_FRAMEWORK()
