#include <executors/executor.h>

#include <signal.h>

DEFINE_DEFAULT_EXECUTOR(AssetsExecutor, STATELESS_EXECUTOR);

static const char* customFunction(const char** args, size_t agumentsNumber);

static void deleter(char* ptr);

DEFINE_EXECUTOR_METHOD(AssetsExecutor, GET_METHOD, request, response)
{
	json_parser_t parser;
	const char* fileName;
	const char extension[] = "wfdp";
	query_parameter_t* queryParameters;
	size_t queryParametersSize;

	wf_get_request_json(request, &parser);
	wf_get_query_parameters(request, &queryParameters, &queryParametersSize);

	wf_get_json_parser_string(parser, "fileName", true, &fileName);

	dynamic_pages_variable_t* variables = (dynamic_pages_variable_t*)malloc(queryParametersSize * sizeof(dynamic_pages_variable_t));

	if (!variables)
	{
		raise(SIGSEGV);

		return;
	}

	for (size_t i = 0; i < queryParametersSize; i++)
	{
		variables[i].name = queryParameters[i].key;
		variables[i].value = queryParameters[i].value;
	}

	size_t fullNameSize = 1024;
	char* const fullName = (char* const)calloc(fullNameSize, sizeof(char));

	if (!fullName)
	{
		raise(SIGSEGV);

		return;
	}

	snprintf(fullName, fullNameSize, "%s.%s", fileName, extension);

	wf_send_dynamic_file(request, fullName, response, variables, queryParametersSize, false, "");

	free(variables);
	free(fullName);
}

DEFINE_EXECUTOR_METHOD(AssetsExecutor, POST_METHOD, request, response)
{
	wf_register_wfdp_function(request, "customFunction", customFunction, deleter);
}

DEFINE_EXECUTOR_METHOD(AssetsExecutor, DELETE_METHOD, request, response)
{
	wf_unregister_wfdp_function(request, "customFunction");
}

const char* customFunction(const char** args, size_t agumentsNumber)
{
	size_t bufferSize = 1024;
	char* const buffer = (char* const)calloc(bufferSize, sizeof(char));

	if (!buffer)
	{
		raise(SIGSEGV);

		return "";
	}

	snprintf(buffer, bufferSize, "Data: %s %s %s", args[0], args[1], args[2]);

	return buffer;
}

void deleter(char* ptr)
{
	free(ptr);
}

DEFINE_INITIALIZE_WEB_FRAMEWORK();
