#pragma once

#include "SQLValue.h"

typedef void* SQLResult;

WebFrameworkException getSQLResultSize(SQLResult implementation, size_t* result);

WebFrameworkException iterateSQLResult(SQLResult implementation, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char* columnName, const SQLValue columnValue, size_t index, size_t size, void* buffer), void* buffer);
