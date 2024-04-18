#pragma once

#include <string>
#include <chrono>
#include <memory>
#include <cstdint>

/// <summary>
/// Default HTTP port
/// </summary>
inline constexpr std::string_view HTTPPort = "80";
/// <summary>
/// Default HTTPS port
/// </summary>
inline constexpr std::string_view HTTPSPort = "443";

inline const std::string contentLengthHeader = "Content-Length";
inline const std::string crlfcrlf = "\r\n\r\n";

/// <summary>
/// <para>Session life time for client session in nanoseconds</para>
/// <para>If session stands equals or more this value, it must be deleted</para>
/// </summary>
inline constexpr uint64_t sessionLifeTime = 10 * 60 * 1'000'000'000ULL;
/// <summary>
/// Check session life time period in seconds
/// </summary>
inline constexpr std::chrono::duration<uint32_t> sessionCheckPeriod(60);

inline const std::string webFrameworkDefaultAssests = "WebFrameworkAssets";

inline constexpr std::string_view webFrameworkDynamicPagesExtension = ".wfdp";

inline constexpr size_t defaultChunkSize = 14 * 1024 * 1024;

namespace json_settings
{
	inline constexpr std::string_view webServerObject = "WebServer";
	inline constexpr std::string_view webFrameworkObject = "WebFramework";
	inline constexpr std::string_view httpsObject = "HTTPS";
	inline constexpr std::string_view loggingObject = "Logging";
	inline constexpr std::string_view webFrameworkLoadBalancerObject = "WebFrameworkLoadBalancer";
	inline constexpr std::string_view threadPoolServerObject = "ThreadPoolServer";

	inline constexpr std::string_view settingsPathsKey = "settingsPaths";
	inline constexpr std::string_view ipKey = "ip";
	inline constexpr std::string_view portKey = "port";
	inline constexpr std::string_view timeoutKey = "timeout";
	inline constexpr std::string_view loadSourcesKey = "loadSources";
	inline constexpr std::string_view assetsPathKey = "assetsPath";
	inline constexpr std::string_view templatesPathKey = "templatesPath";
	inline constexpr std::string_view cachingSize = "cachingSize";
	inline constexpr std::string_view dateFormatKey = "dateFormat";
	inline constexpr std::string_view logFileSizeKey = "logFileSize";
	inline constexpr std::string_view webServerTypeKey = "webServerType";
	inline constexpr std::string_view useHTTPSKey = "useHTTPS";
	inline constexpr std::string_view pathToCertificateKey = "pathToCertificate";
	inline constexpr std::string_view pathToKey = "pathToKey";
	inline constexpr std::string_view webFrameworkDefaultAssetsPath = "defaultAssetsPath";

	inline constexpr std::string_view listOfServersKey = "listOfServers";
	inline constexpr std::string_view loadBalancerIpKey = "loadBalancerIp";
	inline constexpr std::string_view loadBalancerPortKey = "loadBalancerPort";
	inline constexpr std::string_view loadBalancerTimeoutKey = "loadBalancerTimeout";

	inline constexpr std::string_view usingLoggingKey = "usingLogging";
	inline constexpr std::string_view logsPathKey = "logsPath";

	inline constexpr std::string_view defaultLoadSourceValue = "current";

	inline constexpr std::string_view multiThreadedWebServerTypeValue = "multiThreaded";
	inline constexpr std::string_view threadPoolWebServerTypeValue = "threadPool";
}

namespace exceptions
{
	inline constexpr std::string_view notImplemented = "Not implemented";
	inline constexpr std::string_view fileDoesNotExist = "file does not exist";
	inline constexpr std::string_view cantFindFunction = "Can't find ";
	inline constexpr std::string_view useMacroToMakeCreationFunction = "Make sure that you add DECLARE_EXECUTOR macro for executor class";
	inline constexpr std::string_view missingLoadType = "Missing load type of ";
	inline constexpr std::string_view cantLoadSource = "Can't load ";
	inline constexpr std::string_view missingOtherDLLs = "That load source requires to load other dlls";

	inline const std::string wrongWebServerType = "Wrong type of webServerType option";

	inline constexpr std::string_view variableDeclarationSyntaxError = "Wrong variable declaration";
	inline constexpr std::string_view sectionDeclarationSyntaxError = "Wrong section declaration";
	inline constexpr std::string_view missingSemicolonSyntaxError = "Missing semicolon";

	inline constexpr std::string_view stringConversionErrorCode = "String conversion error code ";

	inline constexpr std::string_view badRequest = "Bad request from client";
}

namespace json_settings_values
{
	inline constexpr std::string_view initializationLoadTypeValue = "initialization";
	inline constexpr std::string_view dynamicLoadTypeValue = "dynamic";
}

namespace web_framework_assets
{
	inline constexpr std::string_view errorsFolder = "Errors";

	inline constexpr std::string_view badRequest = "400.html";
	inline constexpr std::string_view notFound = "404.html";
	inline constexpr std::string_view internalServerError = "500.html";
}

#ifdef WEB_FRAMEWORK_DLL
template<typename T>
using smartPointer = std::shared_ptr<T>;
#ifdef __LINUX__
#define WEB_FRAMEWORK_API __attribute__((visibility("default")))
#else
#define WEB_FRAMEWORK_API __declspec(dllexport)
#endif

#define WEB_FRAMEWORK_API_FUNCTION extern "C" WEB_FRAMEWORK_API
#else
template<typename T>
using smartPointer = std::unique_ptr<T>;
#define WEB_FRAMEWORK_API 
#define WEB_FRAMEWORK_API_FUNCTION 
#endif // WEB_FRAMEWORK_DLL
