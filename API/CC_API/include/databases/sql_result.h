#pragma once

#include "sql_value.h"

typedef void* sql_result_t;

/**
 * @brief Get rows in sql_result_t
 * @param implementation sql_result_t instance
 * @param result Rows in sql_result_t
 * @return Error if occurred
 */
web_framework_exception_t wf_get_sql_result_size(sql_result_t implementation, size_t* result);

/**
 * @brief Iterate through all rows in sql_result_t
 * @param implementation sql_result_t instance
 * @param initBuffer Callback that uses for initializing buffer for storing sql_result_t results
 * @param callback Callback that called for each row
 * @param buffer Your custom data
 * @return Error if occurred
 */
web_framework_exception_t wf_iterate_sql_result(sql_result_t implementation, void(*init_buffer)(size_t size, void* buffer), void(*callback)(const char** column_names, const sql_value_t* column_values, size_t size, size_t index, void* buffer), void* buffer);
