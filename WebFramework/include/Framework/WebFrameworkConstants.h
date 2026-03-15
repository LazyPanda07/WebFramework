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
	inline constexpr std::string_view taskBrokerObject = "TaskBroker";

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
	inline constexpr std::string_view databasesKey = "databases";
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

	inline constexpr std::string_view taskExecutorsSettingsKey = "taskExecutorsSettings";
	inline constexpr std::string_view consumerKey = "consumer";
	inline constexpr std::string_view taskBrokersKey = "taskBrokers";
	inline constexpr std::string_view consumerThreadsKey = "consumerThreads";
	inline constexpr std::string_view checkPeriodKey = "checkPeriod";
	inline constexpr std::string_view taskExecutorsSourcePathKey = "sourcePath";
	inline constexpr std::string_view taskExecutorsClassNamesKey = "classNames";
	inline constexpr std::string_view taskExecutorsApiKey = "api";

	inline constexpr std::string_view rabbitMqHostKey = "host";
	inline constexpr std::string_view rabbitMqPortKey = "port";
}

namespace exceptions
{
	inline constexpr std::string_view fileDoesNotExist = "file does not exist";
	inline constexpr std::string_view cantFindFunction = "Can't find";
	inline constexpr std::string_view useMacroToMakeCreationFunction = "Make sure that you add DEFINE_EXECUTOR macro for executor class";
	inline constexpr std::string_view missingLoadType = "Missing load type of";
	inline constexpr std::string_view cantLoadSource = "Can't load";
	inline constexpr std::string_view missingOtherDLLs = "That load source requires to load other dlls";
	inline constexpr std::string_view notCXXLibrary = "That load source is not cxx";
	inline constexpr std::string_view notCCLibrary = "That load source is not cc";

	inline const std::string wrongWebServerType = "Wrong type of webServerType option";

	inline constexpr std::string_view variableDeclarationSyntaxError = "Wrong variable declaration";
	inline constexpr std::string_view sectionDeclarationSyntaxError = "Wrong section declaration";
	inline constexpr std::string_view missingSemicolonSyntaxError = "Missing semicolon";

	inline constexpr std::string_view stringConversionErrorCode = "String conversion error code ";
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

	inline constexpr std::string_view consumerInternalValue = "internal";
	inline constexpr std::string_view consumerExternalValue = "external";
	inline constexpr size_t consumerThreadsDefaultValue = 1;
	inline constexpr size_t checkPeriodDefaultValue = 2500; // in milliseconds

	inline constexpr std::string_view rabbitMqHostValue = "localhost";
	inline constexpr int rabbitMqPortValue = 5672;
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

namespace framework::logging
{
	namespace category
	{
		inline constexpr char loadSource[] = "LogLoadSource";
		inline constexpr char webFrameworkServer[] = "LogWebFrameworkServer";
		inline constexpr char loadBalancer[] = "LogLoadBalancer";
		inline constexpr char resource[] = "LogResource";
		inline constexpr char dynamicFunction[] = "LogDynamicFunction";
		inline constexpr char runtime[] = "LogRuntime";
		inline constexpr char https[] = "LogHTTPS";
		inline constexpr char database[] = "LogDatabase";
		inline constexpr char ccHeuristic[] = "LogCCHeuristic";
		inline constexpr char csharpHeuristic[] = "LogCSharpHeuristic";
		inline constexpr char cxxHeuristic[] = "LogCXXHeuristic";
		inline constexpr char pythonHeuristic[] = "LogPythonHeuristic";
		inline constexpr char filter[] = "LogFilter";
		inline constexpr char executor[] = "LogExecutor";
		inline constexpr char taskExecutor[] = "LogTaskExecutor";
		inline constexpr char taskBroker[] = "LogTaskBroker";
		inline constexpr char ccRuntime[] = "LogCCRuntime";
		inline constexpr char cxxRuntime[] = "LogCXXRuntime";
		inline constexpr char dotnetRuntime[] = "LogDotNetRuntime";
		inline constexpr char pythonRuntime[] = "LogPythonRuntime";
		inline constexpr char largeBodyHandler[] = "LogLargeBodyHandler";
		inline constexpr char segfaultHandler[] = "LogSegfaultHandler";
		inline constexpr char sources[] = "LogSources";
		inline constexpr char executorServer[] = "LogExecutorServer";
	}

	namespace message
	{
		inline constexpr char cantLoadFunction[] = "Can't load: {} function";
		inline constexpr char serverException[] = "Server exception: {}";
		inline constexpr char receiveClientRequestWebError[] = "Receiving client request web error: {}";
		inline constexpr char receiveClientRequestInternalError[] = "Receiving client request internal error: {}";
		inline constexpr char receiveClientRequestUnexpectedError[] = "Some unexpected error acquired while getting client request";
		inline constexpr char sendClientRequestWebError[] = "Sending client request web error: {}";
		inline constexpr char sendClientRequestInternalError[] = "Sending client request internal error: {}";
		inline constexpr char sendClientRequestUnexpectedError[] = "Some unexpected error acquired while sending client request";
		inline constexpr char receiveServerRequestWebError[] = "Receiving server response web error: {}";
		inline constexpr char receiveServerRequestInternalError[] = "Receiving server response internal error: {}";
		inline constexpr char receiveServerRequestUnexpectedError[] = "Some unexpected error acquired while getting server response";
		inline constexpr char sendServerRequestWebError[] = "Sending client response web error: {}";
		inline constexpr char sendServerRequestInternalError[] = "Sending client response internal error: {}";
		inline constexpr char sendServerRequestUnexpectedError[] = "Some unexpected error acquired while sending client response";
		inline constexpr char wrongRuntime[] = "Wrong runtime: {}";
		inline constexpr char cantFindCCHeuristic[] = "Can't find create{}CCHeuristic function";
		inline constexpr char cantCreateCCHeuristic[] = "Can't create create{}CCHeuristic heuristic";
		inline constexpr char cantFindCSharpHeuristicSource[] = "Can't find {}";
		inline constexpr char cantFindCXXHeuristic[] = "Can't find create{}CXXHeuristic function";
		inline constexpr char cantCreateCXXHeuristic[] = "Can't create create{}CXXHeuristic heuristic";
		inline constexpr char cantFindPythonHeuristicSource[] = "Can't find {} in {}";
		inline constexpr char cantCreateApiExecutor[] = "Can't find creator for executor: {} with API: {}";
		inline constexpr char cantLoadTaskExecutor[] = "Can't load task executors from {}";
		inline constexpr char dotnetError[] = ".NET error: {}";
		inline constexpr char rabbitmqLoginError[] = "RabbitMQ login exception: {}";
		inline constexpr char amqpFunctionCallException[] = "AMQP function call exception: {}";
		inline constexpr char cantProcessTask[] = "Can't process task: {}. TaskBroker: {}";
		inline constexpr char largeBodyChunkException[] = "Exception on handle chunk: {}";
		inline constexpr char segfaultMessage[] = "Segmentation fault: {}";
		inline constexpr char cantFindSource[] = "Can't find source {}";
		inline constexpr char cantLoadSource[] = "Can't load source {}, {}";
		inline constexpr char cantGetOrCreateModel[] = "Can't get or create model in HTTPRequest::getModel<T> function where T is {}";
		inline constexpr char executorsServeException[] = "Executors serve exception: {}";
		inline constexpr char badRequest[] = "Bad request from client: {}";
		inline constexpr char cantFindFile[] = "Can't find file exception: {}";
		inline constexpr char notFound[] = "Not found exception: {}";
		inline constexpr char executorInternalServer[] = "Executor internal server error: {}";
		inline constexpr char internalServerError[] = "Internal server error: {}";
		inline constexpr char sslException[] = "SSL exception: {}, ip: {}";

		inline constexpr char heuristicSelect[] = "Select {}:{} server for connection with heuristic value: {}";
		inline constexpr char requestStaticFile[] = "Request static file: {}, is binary: {}";
		inline constexpr char requestDynamicFile[] = "Request dynamic file: {}, is binary: {}";
		inline constexpr char registerFunction[] = "Register function: {} from: {}";
		inline constexpr char addRuntime[] = "Add {} runtime";
		inline constexpr char httpsInitialization[] = "Using HTTPS with certificate: {}, key: {}";
		inline constexpr char databaseInitialization[] = "Using {} database";
		inline constexpr char startingServer[] = "Starting {} server at {}:{}";
		inline constexpr char stoppingServer[] = "Stopping {} server at {}:{}";
		inline constexpr char kickClient[] = "Kick client with ip: {} from server";
		inline constexpr char wrongUserAgent[] = "Wrong User-Agent: {}";
		inline constexpr char noUserAgent[] = "No User-Agent provided";
		inline constexpr char foundTaskExecutorSource[] = "Found load source with path: {}";
		inline constexpr char callDynamicFunction[] = "Call {} function";
		inline constexpr char foundExecutor[] = "Found {} in {} for {} route";
		inline constexpr char foundTaskExecutor[] = "Found {} in {}";
		inline constexpr char foundCSharpApi[] = "Found {}";
		inline constexpr char initializePythonInterpreter[] = "Initialize Python interpreter";
		inline constexpr char pythonInterpreterAlreadyInitialized[] = "Python interpreter already initialized";
		inline constexpr char loadPythonSource[] = "Import {} from {}";
		inline constexpr char openAmqpSocket[] = "Open AMQP socket with {} on {}";
		inline constexpr char rabbitmqLogin[] = "Login to RabbitMQ from channel {}";
		inline constexpr char startTaskConsuming[] = "Start task consuming";
		inline constexpr char addTaskBroker[] = "Add task broker: {}";
	}
}
