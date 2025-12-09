#include <Executors/Executor.h>

#include <Utility/WebFrameworkUtility.h>

DEFINE_DEFAULT_EXECUTOR(DynamicResources, HEAVY_OPERATION_STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(DynamicResources, GET_METHOD, request, response)
{
	const char* fileData;
	const char* result;
	size_t size;
	size_t resultSize;

	getFile(request, "page.md", &fileData, &size);

	processStaticFile(request, fileData, size, ".md", &result, &resultSize);

	setBody(response, result);

	free(fileData);
	free(result);
}

DEFINE_EXECUTOR_METHOD(DynamicResources, POST_METHOD, request, response)
{
	const char* fileData;
	const char* result;
	size_t size;
	size_t resultSize;
	JSONParser parser;
	DynamicPagesVariable_t variable = { .name = "data" };

	getHTTPRequestJSON(request, &parser);

	getFile(request, "print.wfdp", &fileData, &size);
	getJSONParserString(parser, "data", true, &variable.value);

	processWFDPFile(request, fileData, size, &variable, 1, &result, &resultSize);

	setBody(response, result);

	free(fileData);
	free(result);
}
