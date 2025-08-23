#include <APIExecutors/BaseExecutor.h>

#include <JSONBuilder.h>
#include <Utility/WebFrameworkLocalization.h>

DECLARE_DEFAULT_EXECUTOR(LocalizationExecutor, STATELESS_EXECUTOR);

DECLARE_EXECUTOR_METHOD(LocalizationExecutor, GET_METHOD, request, response)
{
	JSONBuilder builder;
	JSONParser parser;
	const char* value;
	const char* language;

	createJSONBuilder(&builder);
	getHTTPRequestJSON(request, &parser);

	getJSONParserString(parser, "language", true, &language);
	getLocalizedString("LocalizationData", "key", language, &value);

	appendJSONBuilderString(builder, "result", value);

	setJSONBody(response, builder);

	deleteWebFrameworkJSONBuider(builder);
}
