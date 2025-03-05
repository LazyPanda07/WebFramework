#pragma once

#ifdef __LINUX__
#define EXECUTORS_API __attribute__((visibility("default")))
#else
#define EXECUTORS_API __declspec(dllexport)

#pragma warning(disable: 4275)
#pragma warning(disable: 4251)
#endif
