#pragma once

#include <string>
#include <chrono>

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

	inline const std::string settingsPathKey = "settingsPath";
	inline const std::string portKey = "port";
	inline const std::string timeoutKey = "timeout";
	inline const std::string loadSourceKey = "loadSource";
	inline const std::string assetsPathKey = "assetsPath";
	inline const std::string usingAssetsCacheKey = "usingAssetsCache";
	inline const std::string usingLoggingKey = "usingLogging";
	inline const std::string dateFormatKey = "dateFormat";
	inline const std::string addNewLineAfterLogKey = "addNewLineAfterLog";

	inline const std::string defaultLoadSourceValue = "current";
	inline const std::string defaultUsingAssetsCache = "false";
}

namespace exceptions
{
	inline const std::string notImplemented = "Not implemented";
	inline const std::string fileDoesNotExist = "file does not exist";
	inline const std::string cantFindFunction = "Can't find ";
	inline const std::string missingLoadType = "Missing load type of ";
	inline const std::string cantLoadSource = "Can't load ";

	inline const std::string cantFindSettingsPath = "Can't find settings path option";
	inline const std::string cantFindAssetsPath = "Can't find assets path option";
	inline const std::string cantFindUsingAssetsCache = "Can't find using assets cache option";
	inline const std::string cantFindLoadSource = "Can't find load source option";

	inline const std::string cantFindPort = "Can't find port option";
	inline const std::string cantFindTimeout = "Can't find timeout option";

	inline const std::string cantFindDateFormat = "Can't find date format option";

	inline const std::string variableDeclarationSyntaxError = "Wrong variable declaration";
	inline const std::string sectionDeclarationSyntaxError = "Wrong section declaration";
	inline const std::string missingSemicolonSyntaxError = "Missing semicolon";
}

namespace XMLSettingsTags
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

namespace XMLSettingsValues
{
	inline const std::string initializationLoadTypeValue = "initialization";
	inline const std::string dynamicLoadTypeValue = "dynamic";
}

namespace WebFrameworkAssets
{
	inline constexpr std::string_view errorsFolder = "Errors";

	inline constexpr std::string_view badRequest = "400.html";
	inline constexpr std::string_view notFound = "404.html";
	inline constexpr std::string_view internalServerError = "500.html";
}