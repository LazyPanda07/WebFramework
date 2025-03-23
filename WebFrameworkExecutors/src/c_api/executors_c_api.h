#pragma once

#include <stdint.h>

#ifdef __LINUX__
#define EXPORT extern "C" __attribute__((visibility("default")))
#else
#define EXPORT extern "C" __declspec(dllexport)
#endif

typedef void* Executor;

EXPORT int getExecutorType(Executor executor, const char* executorName);

EXPORT void destroy(Executor executor, const char* executorName);
