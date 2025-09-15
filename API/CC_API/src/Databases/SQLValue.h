#pragma once

#include "../DLLHandler.h"

typedef void* SQLValue;

/**
 * @brief Type of value
 */
typedef enum SQLValueType
{
	/**
	 * @brief int64_t
	 */
	INT_TYPE,
	/**
	 * @brief double
	 */
	DOUBLE_TYPE,
	/**
	 * @brief const char*
	 */
	STRING_TYPE,
	/**
	 * @brief bool
	 */
	BOOL_TYPE,
	/**
	 * @brief NULL
	 */
	NULL_TYPE,
	/**
	 * @brief uint8_t*
	 */
	BLOB_TYPE
} SQLValueType_t;

/**
 * @brief Create instance of SQLValue. Delete with deleteWebFrameworkSQLValue function
 * @param result SQLValue instance
 * @return Error if occurred
 */
WebFrameworkException createSQLValue(SQLValue* result);

/**
 * @brief Set int64_t value to SQLValue
 * @param implementation SQLValue instance
 * @param value Actual value
 * @return Error if occurred
 */
WebFrameworkException setSQLValueInt(SQLValue implementation, int64_t value);

/**
 * @brief Set double value to SQLValue
 * @param implementation SQLValue instance
 * @param value Actual value
 * @return Error if occurred
 */
WebFrameworkException setSQLValueDouble(SQLValue implementation, double value);

/**
 * @brief Set const char* value to SQLValue
 * @param implementation SQLValue instance
 * @param value Actual value
 * @return Error if occurred
 */
WebFrameworkException setSQLValueString(SQLValue implementation, const char* value);

/**
 * @brief Set bool value to SQLValue
 * @param implementation SQLValue instance
 * @param value Actual value
 * @return Error if occurred
 */
WebFrameworkException setSQLValueBool(SQLValue implementation, bool value);

/**
 * @brief Set NULL value to SQLValue
 * @param implementation SQLValue instance
 * @return Error if occurred
 */
WebFrameworkException setSQLValueNull(SQLValue implementation);

/**
 * @brief Set double value to SQLValue
 * @param implementation SQLValue instance
 * @param value Actual value
 * @param size Actual value size
 * @return Error if occurred
 */
WebFrameworkException setSQLValueBlob(SQLValue implementation, const uint8_t* value, size_t size);

/**
 * @brief Get int64_t value from SQLValue
 * @param implementation SQLValue instance
 * @param result int64_t value
 * @return Error if occurred
 */
WebFrameworkException getSQLValueInt(SQLValue implementation, int64_t* result);

/**
 * @brief Get double value from SQLValue
 * @param implementation SQLValue instance
 * @param result double value
 * @return Error if occurred
 */
WebFrameworkException getSQLValueDouble(SQLValue implementation, double* result);

/**
 * @brief Get const char* value from SQLValue
 * @param implementation SQLValue instance
 * @param result const char* value
 * @return Error if occurred
 */
WebFrameworkException getSQLValueString(SQLValue implementation, const char** result);

/**
 * @brief Get bool value from SQLValue
 * @param implementation SQLValue instance
 * @param result bool value
 * @return Error if occurred
 */
WebFrameworkException getSQLValueBool(SQLValue implementation, bool* result);

/**
 * @brief Get uint8_t* value from SQLValue
 * @param implementation SQLValue instance
 * @param result uint8_t* value
 * @param size Size of value
 * @return Error if occurred
 */
WebFrameworkException getSQLValueBlob(SQLValue implementation, const uint8_t** result, size_t* size);

/**
 * @brief Get SQLValueType from SQLValue
 * @param implementation SQLValue instance
 * @param result SQLValueType
 * @return Error if occurred
 */
WebFrameworkException getSQLValueType(SQLValue implementation, SQLValueType_t* result);

/**
 * @brief Delete SQLValue
 * @param implementation SQLValue instance 
 */
void deleteWebFrameworkSQLValue(SQLValue implementation);
