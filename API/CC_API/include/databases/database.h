#pragma once

#include "table.h"

typedef void* database_t;

/**
 * @brief Tries to get table from database or create if can't get
 * @param implementation database_t instance
 * @param tableName Name of table
 * @param createTableQuery Create table query if can't get table
 * @param result table_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_or_create_table(database_t implementation, const char* table_name, const char* create_table_query, table_t* result);

/**
 * @brief Get table from database
 * @param implementation database_t instance
 * @param tableName Name of table
 * @param result table_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_table(database_t implementation, const char* table_name, table_t* result);

/**
 * @brief Check if table exists in database
 * @param implementation database_t instance
 * @param tableName Name of table
 * @param outTable Optional parameter. Also receive table_t instance
 * @param result Does table contain table
 * @return Error if occurred
 */
web_framework_exception_t wf_contains_table(database_t implementation, const char* table_name, table_t* out_table, bool* result);

/**
 * @brief Get database name
 * @param implementation database_t instance
 * @param result database_t name
 * @return Error if occurred
 */
web_framework_exception_t wf_get_database_name(database_t implementation, const char** result);

/**
 * @brief Get database file name
 * @param implementation database_t instance
 * @param result database_t file name
 * @return Error if occurred
 */
web_framework_exception_t wf_get_database_file_name(database_t implementation, const char** result);
