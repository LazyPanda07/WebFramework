#pragma once

#include <databases/sql_value.h>

void initBuffer(size_t size, void* buffer);

void callback(const char** columnNames, const sql_value_t* columnValues, size_t size, size_t index, void* buffer);
