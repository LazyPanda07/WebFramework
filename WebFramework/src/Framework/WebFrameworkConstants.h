#pragma once

#include <string>
#include <chrono>
#include <memory>
#include <cstdint>

inline const std::string webFrameworkDefaultAssests = "WebFrameworkAssets";

namespace json_settings
{
	inline constexpr std::string_view webServerObject = "WebServer";
	inline constexpr std::string_view webFrameworkObject = "WebFramework";
	inline constexpr std::string_view httpsObject = "HTTPS";
	inline constexpr std::string_view loggingObject = "Logging";
	inline constexpr std::string_view threadPoolServerObject = "ThreadPoolServer";
	inline constexpr std::string_view loadBalancerObject = "LoadBalancer";
	inline constexpr std::string_view proxyObject = "Proxy";

	inline constexpr std::string_view settingsPathsKey = "settingsPaths";
	inline constexpr std::string_view loadSourcesKey = "loadSources";
	inline constexpr std::string_view additionalConfigsKey = "additionalConfigs";
	inline constexpr std::string_view ipKey = "ip";
	inline constexpr std::string_view portKey = "port";
	inline constexpr std::string_view timeoutKey = "timeout";
	inline constexpr std::string_view assetsPathKey = "assetsPath";
	inline constexpr std::string_view templatesPathKey = "templatesPath";
	inline constexpr std::string_view cachingSize = "cachingSize";
	inline constexpr std::string_view dateFormatKey = "dateFormat";
	inline constexpr std::string_view logFileSizeKey = "logFileSize";
	inline constexpr std::string_view logFlagsKey = "flags";
	inline constexpr std::string_view webServerTypeKey = "webServerType";
	inline constexpr std::string_view useHTTPSKey = "useHTTPS";
	inline constexpr std::string_view pathToCertificateKey = "pathToCertificate";
	inline constexpr std::string_view pathToKey = "pathToKey";
	inline constexpr std::string_view webFrameworkDefaultAssetsPath = "defaultAssetsPath";
	inline constexpr std::string_view largeBodySizeThresholdKey = "largeBodySizeThreshold";
	inline constexpr std::string_view largeBodyPacketSizeKey = "largeBodyPacketSize";
	inline constexpr std::string_view databaseImplementationKey = "databaseImplementation";
	inline constexpr std::string_view resourcesThreadsKey = "resourcesThreads";
	inline constexpr std::string_view runtimesKey = "runtimes";

	inline constexpr std::string_view listOfServersKey = "listOfServers";
	inline constexpr std::string_view heuristicKey = "heuristic";
	inline constexpr std::string_view loadSourceKey = "loadSource";
	inline constexpr std::string_view serversHTTPSKey = "serversHTTPS";
	inline constexpr std::string_view processingThreadsKey = "processingThreads";
	inline constexpr std::string_view targetRPSKey = "targetRPS";

	inline constexpr std::string_view usingLoggingKey = "usingLogging";
	inline constexpr std::string_view logsPathKey = "logsPath";
	inline constexpr std::string_view duplicateOutputKey = "duplicateOutput";
	inline constexpr std::string_view duplicateErrorOutputKey = "duplicateErrorOutput";

	inline constexpr std::string_view threadCountKey = "threadCount";
}

namespace exceptions
{
	inline constexpr std::string_view fileDoesNotExist = "file does not exist";
	inline constexpr std::string_view cantFindFunction = "Can't find";
	inline constexpr std::string_view useMacroToMakeCreationFunction = "Make sure that you add DEFINE_EXECUTOR macro for executor class";
	inline constexpr std::string_view missingLoadType = "Missing load type of";
	inline constexpr std::string_view cantLoadSource = "Can't load";
	inline constexpr std::string_view missingOtherDLLs = "That load source requires to load other dlls";

	inline const std::string wrongWebServerType = "Wrong type of webServerType option";

	inline constexpr std::string_view variableDeclarationSyntaxError = "Wrong variable declaration";
	inline constexpr std::string_view sectionDeclarationSyntaxError = "Wrong section declaration";
	inline constexpr std::string_view missingSemicolonSyntaxError = "Missing semicolon";

	inline constexpr std::string_view stringConversionErrorCode = "String conversion error code ";

	inline constexpr std::string_view notFound = "Not found";
}

namespace json_settings_values
{
	inline constexpr size_t largeBodySizeThresholdValue = 165 * 1024 * 1024;
	inline constexpr size_t largeBodyPacketSizeValue = 11 * 1024 * 1024;
	inline constexpr uint32_t targetRPSValue = 5000;

	inline constexpr std::string_view multiThreadedWebServerTypeValue = "multiThreaded";
	inline constexpr std::string_view threadPoolWebServerTypeValue = "threadPool";
	inline constexpr std::string_view loadBalancerWebServerTypeValue = "loadBalancer";
	inline constexpr std::string_view proxyWebServerTypeValue = "proxy";

	inline constexpr std::string_view defaultLoadSourceValue = "current";
	inline constexpr std::string_view defaultHeuristicValue = "Connections";

	inline constexpr std::string_view runtimesPythonValue = "python";
	inline constexpr std::string_view runtimesDotNetValue = ".net";
}

namespace web_framework_assets
{
	inline constexpr std::string_view errorsFolder = "Errors";

	inline constexpr std::string_view badRequest = "400.html";
	inline constexpr std::string_view forbidden = "403.html";
	inline constexpr std::string_view notFound = "404.html";
	inline constexpr std::string_view internalServerError = "500.html";
	inline constexpr std::string_view badGateway = "502.html";
}
