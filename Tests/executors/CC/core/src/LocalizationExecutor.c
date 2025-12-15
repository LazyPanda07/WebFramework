#include <executors/executor.h>

#include <json_parser.h>
#include <utility/web_framework_localization.h>

DEFINE_DEFAULT_EXECUTOR(LocalizationExecutor, STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(LocalizationExecutor, GET_METHOD, request, response)
{
	json_builder_t builder;
	json_parser_t parser;
	const char* localizedString;
	const char* language;

	wf_create_json_builder(&builder);
	wf_get_request_json(request, &parser);

	wf_get_json_parser_string(parser, "language", true, &language);
	wf_get_localized_string("LocalizationData", "key", language, &localizedString);

	wf_append_json_builder_string(builder, "result", localizedString);

	wf_set_json_body(response, builder);

	wf_delete_json_builder(builder);
}
