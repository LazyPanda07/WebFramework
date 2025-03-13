#pragma once

#include <stdint.h>

#ifdef __LINUX__
#define EXPORT extern "C" __attribute__((visibility("default")))
#else
#define EXPORT extern "C" __declspec(dllexport)
#endif


