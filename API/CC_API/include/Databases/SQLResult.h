#pragma once

#include "SQLValue.h"

typedef void* SQLResult;

/**
 * @brief Get rows in SQLResult
 * @param implementation SQLResult instance
 * @param result Rows in SQLResult
 * @return Error if occurred
 */
WebFrameworkException getSQLResultSize(SQLResult implementation, size_t* result);

/**
 * @brief Iterate through all rows in SQLResult
 * @param implementation SQLResult instance
 * @param initBuffer Callback that uses for initializing buffer for storing SQLResult results
 * @param callback Callback that called for each row
 * @param buffer Your custom data
 * @return Error if occurred
 */
WebFrameworkException iterateSQLResult(SQLResult implementation, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char** columnNames, const SQLValue* columnValues, size_t size, size_t index, void* buffer), void* buffer);
