#include "http_request.h"

typedef struct file_buffer
{
	char** data;
	size_t* size;
} file_buffer_t;

static void __init_query_buffer(size_t querySize, void* buffer)
{
	query_parameter_t** temp = (query_parameter_t**)buffer;

	*temp = (query_parameter_t*)malloc((querySize + 1) * sizeof(query_parameter_t)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for query parameter\n");

		exit(1);
	}

	memset(&(*temp)[querySize], 0, sizeof(query_parameter_t)); // fill with zeros last element
}

static void __init_chunks_buffer(size_t size, void* buffer)
{
	http_chunk_t** temp = (http_chunk_t**)buffer;

	*temp = (http_chunk_t*)malloc((size + 1) * sizeof(http_chunk_t)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for chunks\n");

		exit(2);
	}

	memset(&(*temp)[size], 0, sizeof(http_chunk_t)); // fill with zeros last element
}

static void __init_headers_buffer(size_t size, void* buffer)
{
	http_header_t** temp = (http_header_t**)buffer;

	*temp = (http_header_t*)malloc((size + 1) * sizeof(http_header_t)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for HTTP headers\n");

		exit(3);
	}

	memset(&(*temp)[size], 0, sizeof(http_header_t)); // fill with zeros last element
}

static void __init_multiparts_buffer(size_t size, void* buffer)
{
	multipart_t** temp = (multipart_t**)buffer;

	*temp = (multipart_t*)malloc((size + 1) * sizeof(multipart_t)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for multiparts\n");

		exit(4);
	}

	memset(&(*temp)[size], 0, sizeof(multipart_t)); // fill with zeros last element
}

static void __init_cookies_buffer(size_t size, void* buffer)
{
	cookie_t** temp = (cookie_t**)buffer;

	*temp = (cookie_t*)malloc((size + 1) * sizeof(cookie_t)); // + 1 uses as \0

	if (!*temp)
	{
		fprintf(stderr, "Can't allocate memory for cookies\n");

		exit(5);
	}

	memset(&(*temp)[size], 0, sizeof(cookie_t)); // fill with zeros last element
}

static void __add_query_parameter(const char* key, const char* value, size_t index, void* buffer)
{
	query_parameter_t* temp = *(query_parameter_t**)buffer;

	temp[index].key = key;
	temp[index].value = value;
}

static void __add_chunk(const char* chunk, size_t chunkSize, size_t index, void* buffer)
{
	http_chunk_t* temp = *(http_chunk_t**)buffer;

	temp[index].data = chunk;
	temp[index].size = chunkSize;
}

static void __add_header(const char* key, const char* value, size_t index, void* buffer)
{
	http_header_t* temp = *(http_header_t**)buffer;

	temp[index].key = key;
	temp[index].value = value;
}

static void __add_multipart(const char* name, const char* fileName, const char* contentType, const char* data, size_t dataSize, size_t index, void* buffer)
{
	multipart_t* temp = *(multipart_t**)buffer;

	temp[index].name = name;
	temp[index].file_name = fileName;
	temp[index].content_type = contentType;
	temp[index].data = data;
	temp[index].data_size = dataSize;
}

static void __add_cookie(const char* key, const char* value, size_t index, void* buffer)
{
	cookie_t* temp = *(cookie_t**)buffer;

	temp[index].key = key;
	temp[index].value = value;
}

static void __fill_file_buffer(const char* data, size_t size, void* buffer)
{
	file_buffer_t* fileBuffer = (file_buffer_t*)buffer;
	char** result = fileBuffer->data;
	size_t* resultSize = fileBuffer->size;

	*result = malloc(size + 1);

	if (!*result)
	{
		fprintf(stderr, "Can't allocate memory for file buffer\n");

		exit(6);
	}

	(*result)[size] = '\0';
	*resultSize = size;

	memcpy(*result, data, size);
}

web_framework_exception_t wf_get_raw_parameters(http_request_t implementation, const char** rawParameters)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getRawParameters)(void* implementation, void** exception);

	*rawParameters = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRawParameters, &exception);

	return exception;
}

web_framework_exception_t wf_get_method(http_request_t implementation, const char** method)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getMethod)(void* implementation, void** exception);

	*method = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getMethod, &exception);

	return exception;
}

web_framework_exception_t wf_get_query_parameters(http_request_t implementation, query_parameter_t** result, size_t* size)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getQueryParameters)(void* implementation, void(*initQueryBuffer)(size_t querySize, void* buffer), void(*addQueryParameter)(const char* key, const char* value, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getQueryParameters, __init_query_buffer, __add_query_parameter, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(query_parameter_t)];
	query_parameter_t* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, 0, sizeof(query_parameter_t));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(query_parameter_t)))
		{
			*size = index;

			break;
		}

		index++;
	}

	return exception;
}

web_framework_exception_t wf_get_http_version(http_request_t implementation, web_framework_string_t* version)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getVersion)(void* implementation, void** exception);

	*version = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getVersion, &exception);

	return exception;
}

web_framework_exception_t wf_get_http_headers(http_request_t implementation, http_header_t** result, size_t* size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getHeaders)(void* implementation, void(*initHeadersBuffer)(size_t size, void* buffer), void(*addHeader)(const char* key, const char* value, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHeaders, __init_headers_buffer, __add_header, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(http_header_t)];
	http_header_t* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, 0, sizeof(http_header_t));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(http_header_t)))
		{
			*size = index;

			break;
		}

		index++;
	}

	return exception;
}

web_framework_exception_t wf_get_http_header(http_request_t implementation, const char* headerName, const char** result)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getHeader)(void* implementation, const char* headerName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getHeader, headerName, &exception);

	return exception;
}

web_framework_exception_t wf_get_http_body(http_request_t implementation, const char** body, size_t* bodySize)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getBody)(void* implementation, size_t* bodySize, void** exception);

	*body = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getBody, bodySize, &exception);

	return exception;
}

web_framework_exception_t wf_set_attribute(http_request_t implementation, const char* name, const char* value)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setAttribute)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setAttribute, name, value, &exception);

	return exception;
}

web_framework_exception_t wf_get_attribute(http_request_t implementation, const char* name, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getAttribute)(void* implementation, const char* name, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getAttribute, name, &exception);

	return exception;
}

web_framework_exception_t wf_delete_session(http_request_t implementation)
{
	web_framework_exception_t exception = NULL;

	typedef void (*deleteSession)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(deleteSession, &exception);

	return exception;
}

web_framework_exception_t wf_remove_attribute(http_request_t implementation, const char* name)
{
	web_framework_exception_t exception = NULL;

	typedef void (*removeAttribute)(void* implementation, const char* name, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(removeAttribute, name, &exception);

	return exception;
}

web_framework_exception_t wf_get_cookies(http_request_t implementation, cookie_t** result, size_t* size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getCookies)(void* implementation, void(*__initCookiesBuffer)(size_t size, void* buffer), void(*__addCookie)(const char* key, const char* value, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getCookies, __init_cookies_buffer, __add_cookie, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(cookie_t)];
	cookie_t* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, 0, sizeof(cookie_t));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(cookie_t)))
		{
			*size = index;

			break;
		}

		index++;
	}

	return exception;
}

web_framework_exception_t wf_get_multiparts(http_request_t implementation, multipart_t** result, size_t* size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getMultiparts)(void* implementation, void(*initMultipartsBuffer)(size_t size, void* buffer), void(*addMultipart)(const char* name, const char* fileName, const char* contentType, const char* data, size_t dataSize, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getMultiparts, __init_multiparts_buffer, __add_multipart, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(multipart_t)];
	multipart_t* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, 0, sizeof(multipart_t));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(multipart_t)))
		{
			*size = index;

			break;
		}

		index++;
	}

	return exception;
}

web_framework_exception_t wf_get_large_data(http_request_t implementation, const large_data_t** result)
{
	web_framework_exception_t exception = NULL;

	typedef const void* (*getLargeData)(void* implementation, void** exception);

	*result = (const large_data_t*)CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getLargeData, &exception);

	return exception;
}

web_framework_exception_t wf_send_asset_file(http_request_t implementation, const char* filePath, http_response_t response, const dynamic_pages_variable_t* variables, size_t variablesSize, bool isBinary, const char* fileName)
{
	web_framework_exception_t exception = NULL;

	typedef void (*sendAssetFile)(void* implementation, const char* filePath, void* response, const void* variables, size_t variablesSize, bool isBinary, const char* fileName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendAssetFile, filePath, response, variables, variablesSize, isBinary, fileName ? fileName : "", &exception);

	return exception;
}

web_framework_exception_t wf_send_static_file(http_request_t implementation, const char* filePath, http_response_t response, bool isBinary, const char* fileName)
{
	web_framework_exception_t exception = NULL;

	typedef void (*sendStaticFile)(void* implementation, const char* filePath, void* response, bool isBinary, const char* fileName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendStaticFile, filePath, response, isBinary, fileName ? fileName : "", &exception);

	return exception;
}

web_framework_exception_t wf_send_dynamic_file(http_request_t implementation, const char* filePath, http_response_t response, const dynamic_pages_variable_t* variables, size_t variablesSize, bool isBinary, const char* fileName)
{
	web_framework_exception_t exception = NULL;

	typedef void (*sendDynamicFile)(void* implementation, const char* filePath, void* response, const void* variables, size_t variablesSize, bool isBinary, const char* fileName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendDynamicFile, filePath, response, variables, variablesSize, isBinary, fileName ? fileName : "", &exception);

	return exception;
}

web_framework_exception_t wf_stream_file(http_request_t implementation, const char* filePath, http_response_t response, const char* fileName, size_t chunkSize)
{
	web_framework_exception_t exception = NULL;

	typedef void (*streamFile)(void* implementation, const char* filePath, void* response, const char* fileName, size_t chunkSize, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(streamFile, filePath, response, fileName, chunkSize, &exception);

	return exception;
}

web_framework_exception_t wf_register_wfdp_function(http_request_t implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result))
{
	web_framework_exception_t exception = NULL;

	typedef void (*registerWFDPFunction)(void* implementation, const char* functionName, const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result), void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(registerWFDPFunction, functionName, function, deleter, &exception);

	return exception;
}

web_framework_exception_t wf_unregister_wfdp_function(http_request_t implementation, const char* functionName)
{
	web_framework_exception_t exception = NULL;

	typedef void (*unregisterWFDPFunction)(void* implementation, const char* functionName, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(unregisterWFDPFunction, functionName, &exception);

	return exception;
}

web_framework_exception_t wf_is_wfdp_function_registered(http_request_t implementation, const char* functionName, bool* result)
{
	web_framework_exception_t exception = NULL;

	typedef bool (*isDynamicFunctionRegistered)(void* implementation, const char* functionName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isDynamicFunctionRegistered, functionName, &exception);

	return exception;
}

web_framework_exception_t wf_get_request_json(http_request_t implementation, json_parser_t* parser)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getRequestJson)(void* implementation, void** exception);

	*parser = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRequestJson, &exception);

	return exception;
}

web_framework_exception_t wf_get_chunks(http_request_t implementation, http_chunk_t** result, size_t* size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getChunks)(void* implementation, void(*initChunkBuffer)(size_t size, void* buffer), void(*addChunk)(const char* chunk, size_t chunkSize, size_t index, void* buffer), void* buffer, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getChunks, __init_chunks_buffer, __add_chunk, result, &exception);

	if (exception)
	{
		return exception;
	}

	uint8_t emptyBuffer[sizeof(http_chunk_t)];
	http_chunk_t* temp = *result;
	size_t index = 0;

	memset(emptyBuffer, 0, sizeof(http_chunk_t));

	while (true)
	{
		if (!memcmp(&temp[index], emptyBuffer, sizeof(http_chunk_t)))
		{
			*size = index;

			break;
		}

		index++;
	}

	return exception;
}

web_framework_exception_t wf_get_file(http_request_t implementation, const char* filePath, const char** result, size_t* size)
{
	web_framework_exception_t exception = NULL;

	typedef void (*getFile)(void* implementation, const char* filePath, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

	file_buffer_t buffer = { .data = result, .size = size };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getFile, filePath, __fill_file_buffer, &buffer, &exception);

	return exception;
}

web_framework_exception_t wf_process_static_file(http_request_t implementation, const char* fileData, size_t size, const char* fileExtension, const char** result, size_t* resultSize)
{
	web_framework_exception_t exception = NULL;

	typedef void (*processStaticFile)(void* implementation, const char* fileData, size_t size, const char* fileExtension, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

	file_buffer_t buffer = { .data = result, .size = resultSize };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(processStaticFile, fileData, size, fileExtension, __fill_file_buffer, &buffer, &exception);

	return exception;
}

web_framework_exception_t wf_process_dynamic_file(http_request_t implementation, const char* fileData, size_t size, const dynamic_pages_variable_t* variables, size_t variablesSize, const char** result, size_t* resultSize)
{
	web_framework_exception_t exception = NULL;

	typedef void (*processDynamicFile)(void* implementation, const char* fileData, size_t size, const void* variables, size_t variablesSize, void(*fillBuffer)(const char* data, size_t size, void* buffer), void* buffer, void** exception);

	file_buffer_t buffer = { .data = result, .size = resultSize };

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(processDynamicFile, fileData, size, variables, variablesSize, __fill_file_buffer, &buffer, &exception);

	return exception;
}

web_framework_exception_t wf_get_raw_request(http_request_t implementation, const char** rawRequest)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getRawRequest)(void* implementation, void** exception);

	*rawRequest = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRawRequest, &exception);

	return exception;
}

web_framework_exception_t wf_get_client_ip_v4(http_request_t implementation, web_framework_string_t* ip)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getClientIpV4)(void* implementation, void** exception);

	*ip = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getClientIpV4, &exception);

	return exception;
}

web_framework_exception_t wf_get_server_ip_v4(http_request_t implementation, web_framework_string_t* ip)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getServerIpV4)(void* implementation, void** exception);

	*ip = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getServerIpV4, &exception);

	return exception;
}

web_framework_exception_t wf_get_client_port(http_request_t implementation, uint16_t* port)
{
	web_framework_exception_t exception = NULL;

	typedef uint16_t(*getClientPort)(void* implementation, void** exception);

	*port = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getClientPort, &exception);

	return exception;
}

web_framework_exception_t wf_get_server_port(http_request_t implementation, uint16_t* port)
{
	web_framework_exception_t exception = NULL;

	typedef uint16_t(*getServerPort)(void* implementation, void** exception);

	*port = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getServerPort, &exception);

	return exception;
}

web_framework_exception_t wf_get_or_create_database_request(http_request_t implementation, const char* databaseName, database_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getOrCreateDatabaseRequest)(void* implementation, const char* databaseName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateDatabaseRequest, databaseName, &exception);

	return exception;
}

web_framework_exception_t wf_get_database_request(http_request_t implementation, const char* databaseName, database_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getDatabaseRequest)(void* implementation, const char* databaseName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getDatabaseRequest, databaseName, &exception);

	return exception;
}

web_framework_exception_t wf_get_or_create_table_request(http_request_t implementation, const char* databaseName, const char* tableName, const char* createTableQuery, table_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getOrCreateTableRequest)(void* implementation, const char* databaseName, const char* tableName, const char* createTableQuery, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getOrCreateTableRequest, databaseName, tableName, createTableQuery, &exception);

	return exception;
}

web_framework_exception_t wf_get_table_request(http_request_t implementation, const char* databaseName, const char* tableName, table_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getTableRequest)(void* implementation, const char* databaseName, const char* tableName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getTableRequest, databaseName, tableName, &exception);

	return exception;
}

web_framework_exception_t wf_get_route_integer_parameter(http_request_t implementation, const char* routeParameterName, int64_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef int64_t(*getRouteIntegerParameter)(void* implementation, const char* routeParameterName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRouteIntegerParameter, routeParameterName, &exception);

	return exception;
}

web_framework_exception_t wf_get_route_double_parameter(http_request_t implementation, const char* routeParameterName, double* result)
{
	web_framework_exception_t exception = NULL;

	typedef double(*getRouteDoubleParameter)(void* implementation, const char* routeParameterName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRouteDoubleParameter, routeParameterName, &exception);

	return exception;
}

web_framework_exception_t wf_get_route_string_parameter(http_request_t implementation, const char* routeParameterName, const char** result)
{
	web_framework_exception_t exception = NULL;

	typedef const char* (*getRouteStringParameter)(void* implementation, const char* routeParameterName, void** exception);

	*result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getRouteStringParameter, routeParameterName, &exception);

	return exception;
}

web_framework_exception_t wf_send_chunks(http_request_t implementation, http_response_t response, const char* (*chunkGenerator)(void* data, size_t* size), void* data)
{
	web_framework_exception_t exception = NULL;

	typedef void (*sendChunks)(void* implementation, void* response, const char* (*chunkGenerator)(void* data, size_t* size), void* data, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendChunks, response, chunkGenerator, data, &exception);

	return exception;
}

web_framework_exception_t wf_send_file_chunks(http_request_t implementation, http_response_t response, const char* fileName, const char* (*chunkGenerator)(void* data, size_t* size), void* data)
{
	web_framework_exception_t exception = NULL;

	typedef void (*sendFileChunks)(void* implementation, void* response, const char* fileName, const char* (*chunkGenerator)(void* data, size_t* size), void* data, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(sendFileChunks, response, fileName, chunkGenerator, data, &exception);

	return exception;
}

web_framework_exception_t wf_throw_web_framework_exception(http_request_t implementation, const char* errorMessage, response_codes_t responseCode, const char* logCategory, size_t exceptionHash)
{
	web_framework_exception_t exception = NULL;

	typedef void (*throwWebFrameworkException)(void* implementation, const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionHash, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(throwWebFrameworkException, errorMessage, (int64_t)responseCode, logCategory, exceptionHash, &exception);

	return exception;
}
