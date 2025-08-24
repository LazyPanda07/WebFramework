#include <APIExecutors/BaseExecutor.h>

#include <signal.h>

DECLARE_DEFAULT_EXECUTOR(AssetsExecutor, STATELESS_EXECUTOR);

static const char* customFunction(const char** args, size_t agumentsNumber)
{
	size_t bufferSize = 1024;
	const char* buffer = (const char*)calloc(bufferSize, sizeof(char));

	if (!buffer)
	{
		raise(SIGSEGV);

		return "";
	}

	snprintf(buffer, bufferSize, "Data: %s %s %s", args[0], args[1], args[2]);

	return buffer;
}

static void deleter(const char* ptr)
{
	free(ptr);
}

DECLARE_EXECUTOR_METHOD(AssetsExecutor, GET_METHOD, request, response)
{
	JSONParser parser;
	const char* fileName;
	const char extension[] = "wfdp";
	QueryParameter* queryParameters;
	size_t queryParametersSize;

	getHTTPRequestJSON(request, &parser);
	getQueryParameters(request, &queryParameters, &queryParametersSize);

	getJSONParserString(parser, "fileName", true, &fileName);

	DynamicPagesVariable* variables = (DynamicPagesVariable*)malloc(queryParametersSize * sizeof(DynamicPagesVariable));

	if (!variables)
	{
		raise(SIGSEGV);

		return "";
	}

	for (size_t i = 0; i < queryParametersSize; i++)
	{
		variables[i].name = queryParameters[i].key;
		variables[i].value = queryParameters[i].value;
	}

	size_t fullNameSize = strlen(fileName) + strlen(extension) + 1;
	const char* fullName = (const char*)calloc(fullNameSize, sizeof(char));

	if (!fullName)
	{
		raise(SIGSEGV);

		return "";
	}

	snprintf(fullName, fullNameSize, "%s.%s", fileName, extension);

	sendDynamicFile(request, fullName, response, &variables, queryParametersSize, false, "");

	free(variables);
	free(fullName);
}

DECLARE_EXECUTOR_METHOD(AssetsExecutor, POST_METHOD, request, response)
{
	registerDynamicFunction(request, "customFunction", customFunction, deleter);
}

DECLARE_EXECUTOR_METHOD(AssetsExecutor, DELETE_METHOD, request, response)
{
	unregisterDynamicFunction(request, "customFunction");
}
