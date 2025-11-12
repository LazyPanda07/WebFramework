#include <Executors/BaseExecutor.h>

#include <JSONBuilder.h>
#include <JsonParser.h>
#include <Utility/WebFrameworkLocalization.h>

DEFINE_DEFAULT_EXECUTOR(LocalizationExecutor, STATELESS_EXECUTOR);

DEFINE_EXECUTOR_METHOD(LocalizationExecutor, GET_METHOD, request, response)
{
	JSONBuilder builder;
	JSONParser parser;
	const char* localizedString;
	const char* language;

	createJSONBuilder(&builder);
	getHTTPRequestJSON(request, &parser);

	getJSONParserString(parser, "language", true, &language);
	getLocalizedString("LocalizationData", "key", language, &localizedString);

	appendJSONBuilderString(builder, "result", localizedString);

	setJSONBody(response, builder);

	deleteWebFrameworkJSONBuilder(builder);
}
