#pragma once

#include "sql_result_t.h"

typedef void* table_t;

web_framework_exception_t wf_execute_query(table_t implementation, const char* query, const sql_value_t* values, size_t size, sql_result_t* result);

web_framework_exception_t wf_delete_sql_result(table_t implementation, sql_result_t result);

web_framework_exception_t wf_get_table_name(table_t implementation, const char** result);
