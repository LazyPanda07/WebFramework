#include <Executors/Executor.h>

#include <JsonParser.h>
#include <Utility/WebFrameworkLocalization.h>

DEFINE_DEFAULT_EXECUTOR(LocalizationExecutor, STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(LocalizationExecutor, GET_METHOD, request, response)
{
	JsonBuilder builder;
	JsonParser parser;
	const char* localizedString;
	const char* language;

	createJsonBuilder(&builder);
	getHTTPRequestJson(request, &parser);

	getJsonParserString(parser, "language", true, &language);
	getLocalizedString("LocalizationData", "key", language, &localizedString);

	appendJsonBuilderString(builder, "result", localizedString);

	setJsonBody(response, builder);

	deleteWebFrameworkJsonBuilder(builder);
}
