#pragma once

#include "../dll_handler.h"
#include "../json_object.h"

#define DEFAULT_CHUNK_SIZE 14680064

/**
 * @brief Generate UUID in format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
 * @param result UUID. Delete with wf_delete_string function
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

/**
 * @brief Generates a binary asset file by packaging assets from a directory into the specified output file.
 * @param directory_path Path to the directory containing the assets to be packaged. Must be a valid, null-terminated C string.
 * @param output_path Path to the output binary asset file to create. Must be a valid, null-terminated C string.
 * @param progress_callback Optional callback invoked to report progress. The callback receives a float progress value (typically 0.0 to 1.0), the path of the asset currently being processed, and the user-provided data pointer. May be NULL if progress reporting is not needed.
 * @param data Opaque user pointer passed through to the progress_callback each time it is invoked.
 * @return Error if occurred
 */
web_framework_exception_t wf_generate_binary_asset_file(const char* directory_path, const char* output_path, void(*progress_callback)(float progress, const char* asset_path, void* data), void* data);

/**
 * @brief Creates a JSON Web Token (JWT) from the given JSON payload and expiration time.
 * @param data Pointer to a JSON object containing the claims/payload to include in the token. Must not be NULL.
 * @param expirationTimeInMinutes Expiration time for the token, in minutes from the current time.
 * @param result Output pointer to a web_framework_string_t that will receive the generated JWT string. Must point to a valid web_framework_string_t. Delete with wf_delete_string function
 * @return Error if occurred
 */
web_framework_exception_t wf_create_jwt(json_object_t* data, int64_t expirationTimeInMinutes, web_framework_string_t* result);
