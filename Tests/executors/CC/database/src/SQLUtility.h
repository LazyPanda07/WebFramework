#pragma once

#include <Databases/SQLValue.h>

void initBuffer(size_t size, void* buffer);

void callback(const char** columnNames, const SQLValue* columnValues, size_t size, size_t index, void* buffer);
