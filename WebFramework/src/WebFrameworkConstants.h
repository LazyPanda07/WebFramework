#pragma once

#include <string>

inline constexpr std::string_view HTTPPort = "80";

inline const std::string contentLengthHeader = "content-length";
inline const std::string crlfcrlf = "\r\n\r\n";

inline constexpr int averageHTTPRequestSize = 1500;
