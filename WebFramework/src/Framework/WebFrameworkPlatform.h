#pragma once

#ifdef __LINUX__
#include <dlfcn.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef __LINUX__
#define WEB_FRAMEWORK_API __attribute__((visibility("hidden")))
#else
#define WEB_FRAMEWORK_API
#endif
