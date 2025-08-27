#pragma once

#include "SQLValue.h"

typedef void* SQLResult;

WebFrameworkException getSQLResultSize(SQLResult implementation, size_t* result);

WebFrameworkException iterateSQLResult(SQLResult implementation, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char** columnNames, const SQLValue* columnValues, size_t size, size_t index, void* buffer), void* buffer);
