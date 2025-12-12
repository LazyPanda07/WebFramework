#include <executors/executor.h>

#include <utility/web_framework_utility.h>

DEFINE_DEFAULT_EXECUTOR(DynamicResources, HEAVY_OPERATION_STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(DynamicResources, GET_METHOD, request, response)
{
	const char* fileData;
	const char* result;
	size_t size;
	size_t resultSize;

	wf_get_file(request, "page.md", &fileData, &size);

	wf_process_static_file(request, fileData, size, ".md", &result, &resultSize);

	wf_set_body(response, result);

	free(fileData);
	free(result);
}

DEFINE_EXECUTOR_METHOD(DynamicResources, POST_METHOD, request, response)
{
	const char* fileData;
	const char* result;
	size_t size;
	size_t resultSize;
	json_parser_t parser;
	dynamic_pages_variable_t variable = { .name = "data" };

	wf_get_request_json(request, &parser);

	wf_get_file(request, "print.wfdp", &fileData, &size);
	wf_get_json_parser_string(parser, "data", true, &variable.value);

	wf_process_wfdp_file(request, fileData, size, &variable, 1, &result, &resultSize);

	wf_set_body(response, result);

	free(fileData);
	free(result);
}
