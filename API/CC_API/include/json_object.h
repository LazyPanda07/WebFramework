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
 * @param jsonObject JsonObject
 * @return Error if occurred
 */
web_framework_exception_t wf_create_json_object(json_object_t* jsonObject);

/**
 * @brief Copy JsonObject.
 * @param jsonObject Result. Delete with deleteWebFrameworkJsonObject function
 * @param other Other JsonObject
 * @return Error if occurred
 */
web_framework_exception_t wf_copy_json_object(json_object_t* jsonObject, const json_object_t* other);

/**
 * @brief Set JsonObject to JsonObject
 * @param jsonObject JsonObject instance
 * @param object JsonObject value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_object(json_object_t* jsonObject, json_object_t* object);

/**
 * @brief Set string to JsonObject
 * @param jsonObject JsonObject instance
 * @param value String value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_string(json_object_t* jsonObject, const char* value);

/**
 * @brief Set integer to JsonObject
 * @param jsonObject JsonObject instance
 * @param value Integer value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_integer(json_object_t* jsonObject, int64_t value);

/**
 * @brief Set unsigned integer to JsonObject
 * @param jsonObject JsonObject instance
 * @param value Unsigned integer value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_unsigned_integer(json_object_t* jsonObject, uint64_t value);

/**
 * @brief Set double to JsonObject
 * @param jsonObject JsonObject instance
 * @param value Double value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_double(json_object_t* jsonObject, double value);

/**
 * @brief Set bool to JsonObject
 * @param jsonObject JsonObject instance
 * @param value Bool value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_boolean(json_object_t* jsonObject, bool value);

/**
 * @brief Set NULL to JsonObject
 * @param jsonObject JsonObject instance
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_null(json_object_t* jsonObject);

/**
 * @brief Set JsonArray to JsonObject
 * @param jsonObject JsonObject instance
 * @param value JsonArray value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_json_object_array(json_object_t* jsonObject, const json_object_t* array, size_t arraySize);

/**
 * @brief Add key to Json object and get associated object
 * @param jsonObject JsonObject instance
 * @param key JSON key
 * @param result Associated Json object
 * @return 
 */
web_framework_exception_t wf_assign_json_object(json_object_t* jsonObject, const char* key, json_object_t* result);

/**
 * @brief Add Json object to an array and get associated object
 * @param jsonObject JsonObject instance
 * @param result Associated Json object
 * @return
 */
web_framework_exception_t wf_emplace_json_object(json_object_t* jsonObject, json_object_t* result);

/**
 * @brief Obtains the size of a Json object and writes it to the provided output parameter.
 * @param jsonObject Pointer to the Json object whose size will be determined.
 * @param size Pointer to a size_t where the computed size will be stored on success. If jsonObject is map or array it returns actual size, ULLONG_MAX otherwise
 * @return A web_framework_exception_t value indicating success or failure. On success, *size is set to the object's size.
 */
web_framework_exception_t wf_size_json_object(json_object_t* jsonObject, size_t* size);

/**
 * @brief Delete JsonObject
 * @param jsonObject JsonObject instance
 */
void wf_delete_json_object(json_object_t* jsonObject);
