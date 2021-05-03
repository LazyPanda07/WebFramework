#pragma once

#include <string>
#include <chrono>
#include <memory>

#if defined(EXECUTOR_DLL) || defined(WEB_FRAMEWORK_DLL) || defined(EXCEPTIONS_DLL) || defined(MIDDLEWARE_DLL) || defined(LOAD_BALANCER_DLL) || defined(DATABASES_DLL) || defined(JSON_DLL) || defined(THREAD_POOL_DLL) || defined(ALL_DLL)
#define DLL_BUILD
template<typename T>
using smartPointer = std::shared_ptr<T>;
#else
template<typename T>
using smartPointer = std::unique_ptr<T>;
#endif

/// <summary>
/// Default HTTP port
/// </summary>
inline constexpr std::string_view HTTPPort = "80";
/// <summary>
/// Default HTTPS port
/// </summary>
inline constexpr std::string_view HTTPSPort = "443";

inline const std::string getRequest		= "GET";
inline const std::string postRequest	= "POST";
inline const std::string headRequest	= "HEAD";
inline const std::string putRequest		= "PUT";
inline const std::string deleteRequest	= "DELETE";
inline const std::string optionsRequest = "OPTIONS";
inline const std::string traceRequest	= "TRACE";

inline const std::string contentLengthHeader = "Content-Length";
inline const std::string crlfcrlf = "\r\n\r\n";

inline constexpr int averageHTTPRequestSize = 1500;

/// <summary>
/// <para>Session life time for client session in nanoseconds</para>
/// <para>If session stands equals or more this value, it must be deleted</para>
/// </summary>
inline constexpr auto sessionLifeTime = 10 * 60 * 1'000'000'000ULL;
/// <summary>
/// Check session life time period in seconds
/// </summary>
inline constexpr std::chrono::duration<unsigned int> sessionCheckPeriod(60);

inline const std::wstring webFrameworkDefaultAssests = L"WebFrameworkAssets";

inline constexpr std::string_view webFrameworkDynamicPagesExtension = ".wfdp";

namespace json
{
	inline const std::string webServerObject = "WebServer";
	inline const std::string webFrameworkObject = "WebFramework";
	inline const std::string loggingObject = "Logging";
	inline const std::string webFrameworkLoadBalancerObject = "WebFrameworkLoadBalancer";

	inline const std::string settingsPathsKey = "settingsPaths";
	inline const std::string ipKey = "ip";
	inline const std::string portKey = "port";
	inline const std::string timeoutKey = "timeout";
	inline const std::string loadSourcesKey = "loadSources";
	inline const std::string assetsPathKey = "assetsPath";
	inline const std::string templatesPathKey = "templatesPath";
	inline const std::string usingAssetsCacheKey = "usingAssetsCache";
	inline const std::string dateFormatKey = "dateFormat";
	inline const std::string addNewLineAfterLogKey = "addNewLineAfterLog";
	inline const std::string webServerTypeKey = "webServerType";

	inline const std::string listOfServersKey = "listOfServers";
	inline const std::string loadBalancerIpKey = "loadBalancerIp";
	inline const std::string loadBalancerPortKey = "loadBalancerPort";
	inline const std::string loadBalancerTimeoutKey = "loadBalancerTimeout";

	inline const std::string usingLoggingKey = "usingLogging";

	inline const std::string defaultLoadSourceValue = "current";

	inline const std::string multithreadedWebServerTypeValue = "multithreaded";
	inline const std::string threadPoolWebServerTypeValue = "threadPool";
}

namespace exceptions
{
	inline const std::string notImplemented = "Not implemented";
	inline const std::string fileDoesNotExist = "file does not exist";
	inline const std::string cantFindFunction = "Can't find ";
	inline const std::string useMacroToMakeCreationFunction = "Make sure that you add CREATE_BASE_EXECUTOR_SUBCLASS macro to executor class";
	inline const std::string missingLoadType = "Missing load type of ";
	inline const std::string cantLoadSource = "Can't load ";
	inline const std::string missingOtherDLLs = "That load source requires to load other dlls";

	inline const std::string wrongWebServerType = "Wrong type of webServerType option";

	inline const std::string variableDeclarationSyntaxError = "Wrong variable declaration";
	inline const std::string sectionDeclarationSyntaxError = "Wrong section declaration";
	inline const std::string missingSemicolonSyntaxError = "Missing semicolon";

	inline const std::string stringConversionErrorCode = "String conversion error code ";

	inline const std::string badRequest = "Bad request from client";
}

namespace json_settings_values
{
	inline const std::string initializationLoadTypeValue = "initialization";
	inline const std::string dynamicLoadTypeValue = "dynamic";
}

namespace web_framework_assets
{
	inline constexpr std::string_view errorsFolder = "Errors";

	inline constexpr std::string_view badRequest = "400.html";
	inline constexpr std::string_view notFound = "404.html";
	inline constexpr std::string_view internalServerError = "500.html";
}
