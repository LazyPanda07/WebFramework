#pragma once

#include <cstdint>
#include <chrono>
#include <string_view>

/// <summary>
/// <para>Session life time for client session in nanoseconds</para>
/// <para>If session stands equals or more this value, it must be deleted</para>
/// </summary>
inline constexpr uint64_t sessionLifeTime = 10 * 60 * 1'000'000'000ULL;

/// <summary>
/// Check session life time period in seconds
/// </summary>
inline constexpr std::chrono::duration<uint32_t> sessionCheckPeriod(60);

inline constexpr std::string_view webFrameworkDynamicPagesExtension = ".wfdp";

inline constexpr size_t defaultChunkSize = 14 * 1024 * 1024;

namespace json_settings
{
	inline constexpr std::string_view userAgentFilterKey = "userAgentFilter";
}

//#ifdef __LINUX__
//#define WEB_FRAMEWORK_CORE_API __attribute__((visibility("hidden")))
//#else
//#define WEB_FRAMEWORK_CORE_API
//#endif

#ifdef __LINUX__
#define WEB_FRAMEWORK_CORE_API __attribute__((visibility("default")))
#else
#define WEB_FRAMEWORK_CORE_API __declspec(dllexport)
#endif
