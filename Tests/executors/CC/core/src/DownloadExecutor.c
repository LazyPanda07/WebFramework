#include <executors/executor.h>

#include <utility/web_framework_utility.h>

DEFINE_DEFAULT_EXECUTOR(DownloadExecutor, HEAVY_OPERATION_STATELESS_EXECUTOR)

DEFINE_EXECUTOR_METHOD(DownloadExecutor, GET_METHOD, request, response)
{
	json_parser_t parser;
	const char* fileName;

	wf_get_request_json(request, &parser);

	wf_get_json_parser_string(parser, "fileName", false, &fileName);

	wf_stream_file(request, fileName, response, fileName, DEFAULT_CHUNK_SIZE);
}
