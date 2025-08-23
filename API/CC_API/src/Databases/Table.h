#pragma once

#include "SQLResult.h"

typedef void* Table;

WebFrameworkException executeQuery(Table implementation, const char* query, const SQLValue* values, size_t size, SQLResult* result);

WebFrameworkException deleteSQLResult(Table implementation, SQLResult result);

WebFrameworkException getTableName(Table implementation, const char** result);
