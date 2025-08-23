#pragma once

#include "Table.h"

typedef void* Database;

WebFrameworkException getOrCreateTable(Database implementation, const char* tableName, const char* createTableQuery, Table* result);

WebFrameworkException getTable(Database implementation, const char* tableName, Table* result);

WebFrameworkException containsTable(Database implementation, const char* tableName, Table* outTable, bool* result);

WebFrameworkException getDatabaseName(Database implementation, const char** result);

WebFrameworkException getDatabaseFileName(Database implementation, const char** result);
