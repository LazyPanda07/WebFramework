#pragma once

#include "../DLLHandler.h"

typedef void* SQLValue;

typedef enum
{
	INT_TYPE,
	DOUBLE_TYPE,
	STRING_TYPE,
	BOOL_TYPE,
	NULL_TYPE,
	BLOB_TYPE
} SQLValueType;

WebFrameworkException setSQLValueInt(SQLValue implementation, int64_t value);

WebFrameworkException setSQLValueDouble(SQLValue implementation, double value);

WebFrameworkException setSQLValueString(SQLValue implementation, const char* value);

WebFrameworkException setSQLValueBool(SQLValue implementation, bool value);

WebFrameworkException setSQLValueNull(SQLValue implementation);

WebFrameworkException setSQLValueBlob(SQLValue implementation, const uint8_t* value, size_t size);

WebFrameworkException getSQLValueInt(SQLValue implementation, int64_t* result);

WebFrameworkException getSQLValueDouble(SQLValue implementation, double* result);

WebFrameworkException getSQLValueString(SQLValue implementation, const char** result);

WebFrameworkException getSQLValueBool(SQLValue implementation, bool* result);

WebFrameworkException getSQLValueBlob(SQLValue implementation, const uint8_t** result, size_t* size);

WebFrameworkException getSQLValueType(SQLValue implementation, SQLValueType* result);
