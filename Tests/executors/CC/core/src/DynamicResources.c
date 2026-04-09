#include <executors/executor.h>

#include <utility/web_framework_utility.h>

DEFINE_DEFAULT_EXECUTOR(DynamicResources, HEAVY_OPERATION_STATELESS_EXECUTOR)

DEFINE_EXECUTOR_METHOD(DynamicResources, GET_METHOD, request, response)
{
	char* fileData = NULL;
	char* result = NULL;
	size_t size;
	size_t resultSize;

	wf_get_file(request, "page.md", &fileData, &size);

	wf_process_static_file(request, fileData, size, ".md", &result, &resultSize);

	wf_set_body(response, result, strlen(result));

	free(fileData);
	free(result);
}

DEFINE_EXECUTOR_METHOD(DynamicResources, POST_METHOD, request, response)
{
	char* fileData = NULL;
	char* result = NULL;
	size_t size;
	size_t resultSize;
	json_parser_t parser;
	json_object_t arguments;

	wf_get_request_json(request, &parser);
	wf_create_json_object(&arguments);

	wf_get_file(request, "print.wfdp", &fileData, &size);
	
	{
		{
			json_object_t print;

			wf_assign_or_get_json_object(&arguments, "@print", &print);
			wf_assign_or_get_json_object(&print, "data", NULL);
		}

		char* data = NULL;
		json_object_t print;
		json_object_t jsonData;

		wf_get_json_parser_string(parser, "data", true, &data);

		wf_assign_or_get_json_object(&arguments, "@print", &print);
		wf_assign_or_get_json_object(&print, "data", &jsonData);

		wf_set_json_object_string(&jsonData, data);
	}

	wf_process_dynamic_file(request, fileData, size, &arguments, &result, &resultSize);

	wf_set_body(response, result, resultSize);

	wf_delete_json_object(&arguments);

	free(fileData);
	free(result);
}
