#include "utility/web_framework_utility.h"

web_framework_exception_t wf_generate_uuid(web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*generateWebFrameworkUUID)(void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(generateWebFrameworkUUID, &exception);

	return exception;
}

web_framework_exception_t wf_generate_sha256(const char* data, size_t size, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*generateSha256)(const char* data, size_t size, void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(generateSha256, data, size, &exception);

	return exception;
}

web_framework_exception_t wf_generate_binary_asset_file(const char* directory_path, const char* output_path, void(*progress_callback)(float progress, const char* asset_path, void* data), void* data)
{
	web_framework_exception_t exception = NULL;

	typedef void (*generateBinaryAssetFile)(const char* directoryPath, const char* outputPath, void(*progressCallback)(float progress, const char* assetPath, void* data), void* data, void** exception);

	CALL_WEB_FRAMEWORK_FUNCTION(generateBinaryAssetFile, directory_path, output_path, progress_callback, data, &exception);

	return exception;
}

web_framework_exception_t wf_create_jwt_with_string(json_object_t* data, int64_t expiration_time_in_minutes, const char* jwt_secret_variable_name, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJwtWithString)(void* jsonData, int64_t expirationTimeInMinutes, const char* jwtSecretVariableName, void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(createJwtWithString, data->implementation, expiration_time_in_minutes, jwt_secret_variable_name ? jwt_secret_variable_name : "JWT_SECRET", &exception);

	return exception;
}

web_framework_exception_t wf_create_jwt_with_context(json_object_t* data, int64_t expiration_time_in_minutes, web_framework_t framework_instance, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*createJwtWithContext)(void* jsonData, int64_t expirationTimeInMinutes, void* frameworkInstance, void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(createJwtWithContext, data->implementation, expiration_time_in_minutes, framework_instance.implementation, &exception);

	return exception;
}
