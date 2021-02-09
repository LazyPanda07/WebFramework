#pragma once

#include <string>
#include <chrono>
#include <memory>

#if defined(EXECUTOR_DLL) || defined(WEB_FRAMEWORK_DLL) || defined(EXCEPTIONS_DLL) || defined(MIDDLEWARE_DLL) || defined(LOAD_BALANCER_DLL) || defined(DATABASES_DLL) || defined(JSON_DLL) || defined(ALL_DLL)
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

namespace ini
{
	inline const std::string webServerSection = "WebServer";
	inline const std::string webFrameworkSection = "WebFramework";
	inline const std::string loggingSection = "Logging";
	inline const std::string webFrameworkLoadBalancerSection = "WebFrameworkLoadBalancer";

	inline const std::string settingsPathKey = "settingsPath";
	inline const std::string ipKey = "ip";
	inline const std::string portKey = "port";
	inline const std::string timeoutKey = "timeout";
	inline const std::string loadSourceKey = "loadSource";
	inline const std::string assetsPathKey = "assetsPath";
	inline const std::string templatesPathKey = "templatesPath";
	inline const std::string usingAssetsCacheKey = "usingAssetsCache";
	inline const std::string dateFormatKey = "dateFormat";
	inline const std::string addNewLineAfterLogKey = "addNewLineAfterLog";

	inline const std::string usingLoggingKey = "usingLogging";

	inline const std::string listOfServersKey = "listOfServers";
	inline const std::string loadBalancerIpKey = "loadBalancerIp";
	inline const std::string loadBalancerPortKey = "loadBalancerPort";
	inline const std::string loadBalancerTimeoutKey = "loadBalancerTimeout";

	inline const std::string defaultLoadSourceValue = "current";
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

	inline const std::string cantFindSettingsPath = "Can't find settings path option";
	inline const std::string cantFindAssetsPath = "Can't find assets path option";
	inline const std::string cantFindTemplatesPath = "Can't find templates path option";
	inline const std::string cantFindUsingAssetsCache = "Can't find using assets cache option";
	inline const std::string cantFindLoadSource = "Can't find load source option";

	inline const std::string cantFindIp = "Can't find ip option";
	inline const std::string cantFindPort = "Can't find port option";
	inline const std::string cantFindTimeout = "Can't find timeout option";

	inline const std::string cantFindDateFormat = "Can't find date format option";

	inline const std::string variableDeclarationSyntaxError = "Wrong variable declaration";
	inline const std::string sectionDeclarationSyntaxError = "Wrong section declaration";
	inline const std::string missingSemicolonSyntaxError = "Missing semicolon";

	inline const std::string stringConversionErrorCode = "String conversion error code ";

	inline const std::string cantFindLoadBalancerIp = "Can't find load balancer ip option";
	inline const std::string cantFindLoadBalancerPort = "Can't find load balancer port option";
	inline const std::string cantFindLoadBalancerTimeout = "Can't find load balancer timeout option";

	inline const std::string badRequest = "Bad request from client";
}

namespace xml_settings_tags
{
	inline constexpr std::string_view startExecutorTag = "<executor>";
	inline constexpr std::string_view endExecutorTag = "</executor>";

	inline constexpr std::string_view startInitParamTag = "<init-param>";
	inline constexpr std::string_view endInitParamTag = "</init-param>";

	inline constexpr std::string_view startParamNameTag = "<param-name>";
	inline constexpr std::string_view endParamNameTag = "</param-name>";

	inline constexpr std::string_view startParamValueTag = "<param-value>";
	inline constexpr std::string_view endParamValueTag = "</param-value>";

	inline constexpr std::string_view startRouteTag = "<route>";
	inline constexpr std::string_view endRouteTag = "</route>";

	inline constexpr std::string_view startExecutorNameTag = "<executor-name>";
	inline constexpr std::string_view endExecutorNameTag = "</executor-name>";

	inline constexpr std::string_view startLoadTypeTag = "<load-type>";
	inline constexpr std::string_view endLoadTypeTag = "</load-type>";
}

namespace xml_settings_values
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

namespace route_parameters
{
	inline const std::string basePattern = "/[a-ZA-Z0-9]*";
	inline const std::string intPattern = "/[0-9]*";
}
