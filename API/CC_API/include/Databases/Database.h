#pragma once

#include "Table.h"

typedef void* Database;

/**
 * @brief Tries to get table from database or create if can't get
 * @param implementation Database instance
 * @param tableName Name of table
 * @param createTableQuery Create table query if can't get table
 * @param result Table instance
 * @return Error if occurred
 */
WebFrameworkException getOrCreateTable(Database implementation, const char* tableName, const char* createTableQuery, Table* result);

/**
 * @brief Get table from database
 * @param implementation Database instance
 * @param tableName Name of table
 * @param result Table instance
 * @return Error if occurred
 */
WebFrameworkException getTable(Database implementation, const char* tableName, Table* result);

/**
 * @brief Check if table exists in database
 * @param implementation Database instance
 * @param tableName Name of table
 * @param outTable Optional parameter. Also receive Table instance
 * @param result Does table contain table
 * @return Error if occurred
 */
WebFrameworkException containsTable(Database implementation, const char* tableName, Table* outTable, bool* result);

/**
 * @brief Get database name
 * @param implementation Database instance
 * @param result Database name
 * @return Error if occurred
 */
WebFrameworkException getDatabaseName(Database implementation, const char** result);

/**
 * @brief Get database file name
 * @param implementation Database instance
 * @param result Database file name
 * @return Error if occurred
 */
WebFrameworkException getDatabaseFileName(Database implementation, const char** result);
