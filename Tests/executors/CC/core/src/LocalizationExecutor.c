#include <APIExecutors/BaseExecutor.h>

#include <JSONBuilder.h>
#include <JSONParser.h>
#include <Utility/WebFrameworkLocalization.h>

DECLARE_DEFAULT_EXECUTOR(LocalizationExecutor, STATELESS_EXECUTOR);

DECLARE_EXECUTOR_METHOD(LocalizationExecutor, GET_METHOD, request, response)
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

	deleteWebFrameworkJSONBuider(builder);
}
