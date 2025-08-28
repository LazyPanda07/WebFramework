#pragma once

#include "DLLHandler.h"

typedef void Heuristic;

#ifdef __LINUX__
#define WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API __attribute__((visibility("default"))) __attribute__((used))
#else
#define WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API __declspec(dllexport)
#endif

#define GENERATE_HEURISTIC_BODY() const char* ip; const char* port; bool useHTTPS

#define DECLARE_HEURISTIC(structName) WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API void* create##structName##CCHeuristic(const char* ip, const char* port, bool useHTTPS)	\
{	\
	structName* result = (structName*)malloc(sizeof(structName));	\
	\
	result->ip = ip;	\
	result->port = port;	\
	result->useHTTPS = useHTTPS;	\
	\
	return result;	\
}	\
	\
WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API const char* webFrameworkCCHeuristicGetIp##structName(void* heuristic)	\
{	\
	return ((structName*)heuristic)->ip;	\
}	\
	\
WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API const char* webFrameworkCCHeuristicGetPort##structName(void* heuristic)	\
{	\
	return ((structName*)heuristic)->port;	\
}	\
	\
WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API bool webFrameworkCCHeuristicGetUseHTTPS##structName(void* heuristic)	\
{	\
	return ((structName*)heuristic)->useHTTPS;	\
}	\
	\
WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API void webFrameworkCCDeleteHeuristic##structName(void* heuristic)	\
{	\
	free((structName*)heuristic);	\
}

#define DECLARE_DEFAULT_HEURISTIC(structName) typedef struct { GENERATE_HEURISTIC_STRUCT_BODY(); } structName; DECLARE_HEURISTIC(structName)

#define DECLARE_HEURISTIC_INIT(structName) WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API void webFrameworkCCHeuristicInit##structName(Heuristic* heuristic)

#define DECLARE_HEURISTIC_FUNCTION(structName) WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API uint64_t webFrameworkCCHeuristicOperator##structName(Heuristic* heuristic)

#define DECLARE_HEURISTIC_ON_START(structName) WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API uint64_t webFrameworkCCHeuristicOnStart##structName(Heuristic* heuristic)

#define DECLARE_HEURISTIC_ON_END(structName) WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API uint64_t webFrameworkCCHeuristicOnEnd##structName(Heuristic* heuristic)
