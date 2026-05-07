#pragma once

#include "dll_handler.h"

/**
 * @brief JsonObject
 */
typedef struct json_object
{
	void* implementation;
	bool weak;
} json_object_t;

/**
 * @brief Create JsonObject. Delete with deleteWebFrameworkJsonObject function
 * @param json_object JsonObject
 * @return Error if occurred
 */
web_framework_exception_t wf_create_json_object(json_object_t* json_object);

/**
 * @brief Copy JsonObject.
 * @param json_object Result. Delete with deleteWebFrameworkJsonObject function
 * @param other Other JsonObject
 * @return Error if occurred
 */
web_framework_exception_t wf_copy_json_object(json_object_t* json_object, const json_object_t* other);

/**
 * @brief Set JsonObject to JsonObject
 * @param json_object JsonObject instance
 * @param object JsonObject value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_object(json_object_t* json_object, json_object_t* object);

/**
 * @brief Set string to JsonObject
 * @param json_object JsonObject instance
 * @param value String value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_string(json_object_t* json_object, const char* value);

/**
 * @brief Set integer to JsonObject
 * @param json_object JsonObject instance
 * @param value Integer value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_integer(json_object_t* json_object, int64_t value);

/**
 * @brief Set unsigned integer to JsonObject
 * @param json_object JsonObject instance
 * @param value Unsigned integer value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_unsigned_integer(json_object_t* json_object, uint64_t value);

/**
 * @brief Set double to JsonObject
 * @param json_object JsonObject instance
 * @param value Double value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_double(json_object_t* json_object, double value);

/**
 * @brief Set bool to JsonObject
 * @param json_object JsonObject instance
 * @param value Bool value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_boolean(json_object_t* json_object, bool value);

/**
 * @brief Set NULL to JsonObject
 * @param json_object JsonObject instance
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_null(json_object_t* json_object);

/**
 * @brief Set JsonArray to JsonObject
 * @param json_object JsonObject instance
 * @param value JsonArray value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_array(json_object_t* json_object, const json_object_t* array, size_t array_size);

/**
 * @brief Get JsonObject
 * @param json_object json_object_t instance
 * @param result Result JsonObject
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_object_object(json_object_t* json_object, json_object_t* result);

/**
 * @brief Get string
 * @param json_object json_object_t instance
 * @param result Result string
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_object_string(json_object_t* json_object, const char** result);

/**
 * @brief Get integer
 * @param json_object json_object_t instance
 * @param result Result integer
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_object_integer(json_object_t* json_object, int64_t* result);

/**
 * @brief Get unsigned integer
 * @param json_object json_object_t instance
 * @param result Result unsigned integer
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_object_unsigned_integer(json_object_t* json_object, uint64_t* result);

/**
 * @brief Get double
 * @param json_object json_object_t instance
 * @param result Result double
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_object_double(json_object_t* json_object, double* result);

/**
 * @brief Get bool
 * @param json_object json_object_t instance
 * @param result Result bool
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_object_boolean(json_object_t* json_object, bool* result);

/**
 * @brief Get NULL
 * @param json_object json_object_t instance
 * @param result Is NULL
 * @return Error if occurred
 */
web_framework_exception_t wf_get_json_object_null(json_object_t* json_object, bool* result);

/**
 * @brief Copy JsonArray
 * @param json_object json_object_t instance
 * @param array Valid json_object_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_copy_json_object_array(json_object_t* json_object, json_object_t* array, size_t* array_size);

/**
 * @brief Retrieve the element at the specified index from a JSON array object and store it in the provided result pointer.
 * @param json_object Pointer to a json_object_t that is expected to represent a JSON array. Must not be NULL.
 * @param index Zero-based index of the element to retrieve from the array.
 * @param result Pointer to a json_object_t that will receive the retrieved element on success. Must not be NULL.
 * @return Error if occurred
 */
web_framework_exception_t wf_index_access_json_object_array(json_object_t* json_object, size_t index, json_object_t* result);

/**
 * @brief Add key to Json object and get associated object
 * @param json_object JsonObject instance
 * @param result Associated Json object, may be NULL. Valid until next call
 * @return Error if occurred
 */
web_framework_exception_t wf_assign_or_get_json_object(json_object_t* json_object, const char* key, json_object_t* result);

/**
 * @brief Add Json object to an array and get associated object
 * @param json_object JsonObject instance
 * @param result Associated Json object, may be NULL. Valid until next call
 * @return Error if occurred
 */
web_framework_exception_t wf_emplace_json_object(json_object_t* json_object, json_object_t* result);

/**
 * @brief Obtains the size of a Json object and writes it to the provided output parameter.
 * @param json_object Pointer to the Json object whose size will be determined.
 * @param size Pointer to a size_t where the computed size will be stored on success. If json_object is map or array it returns actual size, ULLONG_MAX otherwise
 * @return A web_framework_exception_t value indicating success or failure. On success, *size is set to the object's size.
 */
web_framework_exception_t wf_size_json_object(json_object_t* json_object, size_t* size);

/**
 * @brief Delete JsonObject
 * @param json_object JsonObject instance
 */
void wf_delete_json_object(json_object_t* json_object);
