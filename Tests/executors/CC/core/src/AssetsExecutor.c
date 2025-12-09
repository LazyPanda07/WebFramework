#include <Executors/Executor.h>

#include <signal.h>

DEFINE_DEFAULT_EXECUTOR(AssetsExecutor, STATELESS_EXECUTOR);

static const char* customFunction(const char** args, size_t agumentsNumber);

static void deleter(char* ptr);

DEFINE_EXECUTOR_METHOD(AssetsExecutor, GET_METHOD, request, response)
{
	JSONParser parser;
	const char* fileName;
	const char extension[] = "wfdp";
	QueryParameter_t* queryParameters;
	size_t queryParametersSize;

	getHTTPRequestJSON(request, &parser);
	getQueryParameters(request, &queryParameters, &queryParametersSize);

	getJSONParserString(parser, "fileName", true, &fileName);

	DynamicPagesVariable_t* variables = (DynamicPagesVariable_t*)malloc(queryParametersSize * sizeof(DynamicPagesVariable_t));

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

	sendWFDPFile(request, fullName, response, variables, queryParametersSize, false, "");

	free(variables);
	free(fullName);
}

DEFINE_EXECUTOR_METHOD(AssetsExecutor, POST_METHOD, request, response)
{
	registerWFDPFunction(request, "customFunction", customFunction, deleter);
}

DEFINE_EXECUTOR_METHOD(AssetsExecutor, DELETE_METHOD, request, response)
{
	unregisterWFDPFunction(request, "customFunction");
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
