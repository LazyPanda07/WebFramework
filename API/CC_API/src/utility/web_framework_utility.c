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
