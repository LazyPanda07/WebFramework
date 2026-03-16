#pragma once

#include "../dll_handler.h"

#define DEFAULT_CHUNK_SIZE 14680064

/**
 * @brief Generate UUID in format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
 * @param result UUID. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
web_framework_exception_t wf_generate_uuid(web_framework_string_t* result);

/**
 * @brief Generate SHA256 hash
 * @param data Data
 * @param size data size
 * @param result 
 * @return Error if occurred
 */
web_framework_exception_t wf_generate_sha256(const char* data, size_t size, web_framework_string_t* result);
