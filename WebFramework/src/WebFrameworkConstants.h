#pragma once

#include <string>

inline constexpr std::string_view HTTPPort = "80";

inline const std::string contentLengthHeader = "Content-Length";
inline const std::string crlfcrlf = "\r\n\r\n";

inline constexpr int averageHTTPRequestSize = 1500;

namespace ini
{
	inline const std::string webServerSection = "WebServer";
	inline const std::string webFrameworkSection = "WebFramework";

	inline const std::string settingsPathKey = "settingsPath";
	inline const std::string portKey = "port";
	inline const std::string timeoutKey = "timeout";
	inline const std::string loadSourceKey = "loadSource";

	inline const std::string defaultLoadSourceValue = "current";
}

namespace exceptions
{
	inline const std::string notImplemented = "Not implemented";
	inline const std::string fileDoesNotExist = "File does not exist";
	inline const std::string cantFindFunction = "Can't find createBaseExecutorSubclass function";
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
}
