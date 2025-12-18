#pragma once

#include "../dll_handler.h"

typedef void* sql_value_t;

/**
 * @brief Type of value
 */
typedef enum sql_value_type
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
} sql_value_type_t;

/**
 * @brief Create instance of sql_value_t. Delete with deleteWebFrameworkSQLValue function
 * @param result sql_value_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_create_sql_value(sql_value_t* result);

/**
 * @brief Set int64_t value to sql_value_t
 * @param implementation sql_value_t instance
 * @param value Actual value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_sql_value_int(sql_value_t implementation, int64_t value);

/**
 * @brief Set double value to sql_value_t
 * @param implementation sql_value_t instance
 * @param value Actual value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_sql_value_double(sql_value_t implementation, double value);

/**
 * @brief Set const char* value to sql_value_t
 * @param implementation sql_value_t instance
 * @param value Actual value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_sql_value_string(sql_value_t implementation, const char* value);

/**
 * @brief Set bool value to sql_value_t
 * @param implementation sql_value_t instance
 * @param value Actual value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_sql_value_bool(sql_value_t implementation, bool value);

/**
 * @brief Set NULL value to sql_value_t
 * @param implementation sql_value_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_set_sql_value_null(sql_value_t implementation);

/**
 * @brief Set double value to sql_value_t
 * @param implementation sql_value_t instance
 * @param value Actual value
 * @param size Actual value size
 * @return Error if occurred
 */
web_framework_exception_t wf_set_sql_value_blob(sql_value_t implementation, const uint8_t* value, size_t size);

/**
 * @brief Get int64_t value from sql_value_t
 * @param implementation sql_value_t instance
 * @param result int64_t value
 * @return Error if occurred
 */
web_framework_exception_t wf_get_sql_value_int(sql_value_t implementation, int64_t* result);

/**
 * @brief Get double value from sql_value_t
 * @param implementation sql_value_t instance
 * @param result double value
 * @return Error if occurred
 */
web_framework_exception_t wf_get_sql_value_double(sql_value_t implementation, double* result);

/**
 * @brief Get const char* value from sql_value_t
 * @param implementation sql_value_t instance
 * @param result const char* value
 * @return Error if occurred
 */
web_framework_exception_t wf_get_sql_value_string(sql_value_t implementation, const char** result);

/**
 * @brief Get bool value from sql_value_t
 * @param implementation sql_value_t instance
 * @param result bool value
 * @return Error if occurred
 */
web_framework_exception_t wf_get_sql_value_bool(sql_value_t implementation, bool* result);

/**
 * @brief Get uint8_t* value from sql_value_t
 * @param implementation sql_value_t instance
 * @param result uint8_t* value
 * @param size Size of value
 * @return Error if occurred
 */
web_framework_exception_t wf_get_sql_value_blob(sql_value_t implementation, const uint8_t** result, size_t* size);

/**
 * @brief Get SQLValueType from sql_value_t
 * @param implementation sql_value_t instance
 * @param result SQLValueType
 * @return Error if occurred
 */
web_framework_exception_t wf_get_sql_value_type(sql_value_t implementation, sql_value_type_t* result);

/**
 * @brief Delete sql_value_t
 * @param implementation sql_value_t instance 
 */
void wf_delete_sql_value(sql_value_t implementation);
