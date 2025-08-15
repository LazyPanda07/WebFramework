#pragma once

#include "SQLValue.h"

typedef void* SQLResult;

WebFrameworkException getSQLResultSize(SQLResult implementation, size_t* result);

WebFrameworkException iterateSQLResult(SQLResult implementation, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char* columnName, const SQLValue columnValue, size_t index, size_t size, void* buffer), void* buffer);

inline WebFrameworkException getSQLResultSize(SQLResult implementation, size_t* result)
{
	WebFrameworkException exception = NULL;

	typedef size_t(*getSQLResultSize)(void* implementation, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getSQLResultSize, &exception);

	return exception;
}

inline WebFrameworkException iterateSQLResult(SQLResult implementation, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char* columnName, const SQLValue columnValue, size_t index, size_t size, void* buffer), void* buffer)
{
	WebFrameworkException exception = NULL;

	typedef void(*iterateSQLResult)(void* implementation, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char* columnName, const SQLValue columnValue, size_t index, size_t size, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(iterateSQLResult, initBuffer, callback, buffer, &exception);

	return exception;
}
