#pragma once

#include "http_response.h"
#include "databases/database.h"

typedef void* http_request_t;

/**
 * @brief URL encoded parameters
 */
typedef struct query_parameter
{
	const char* key;
	const char* value;
} query_parameter_t;

/**
 * @brief Single chunk in request with Transfer-Encoding: chunk header
 */
typedef struct http_chunk
{
	const char* data;
	size_t size;
} http_chunk_t;

/**
 * @brief HTTP header
 */
typedef struct http_header
{
	const char* key;
	const char* value;
} http_header_t;

/**
 * @brief HTTP cookie
 */
typedef struct cookie
{
	const char* key;
	const char* value;
} cookie_t;

/**
 * @brief Variable for processing .wfdp files
 */
typedef struct dynamic_pages_variable
{
	const char* name;
	const char* value;
} dynamic_pages_variable_t;

/**
 * @brief Data part if file passes large files threshold
 */
typedef struct large_data
{
	const char* data_part;
	size_t data_part_size;
	bool is_last_packet;
} large_data_t;

/**
 * @brief Mutilpart data from forms
 */
typedef struct multipart
{
	const char* name;
	/**
	 * @brief May be NULL
	 */
	const char* file_name;
	/**
	 * @brief May be NULL
	 */
	const char* content_type;
	const char* data;
	size_t data_size;
} multipart_t;

/**
 * @brief Raw string with query parameters
 * @param implementation http_request_t instance
 * @param rawParameters Single string with query parameters
 * @return Error if occurred
 */
web_framework_exception_t wf_get_raw_parameters(http_request_t implementation, const char** raw_parameters);

/**
 * @brief Get HTTP method
 * @param implementation http_request_t instance
 * @param method HTTP method
 * @return Error if occurred
 */
web_framework_exception_t wf_get_method(http_request_t implementation, const char** method);

/**
 * @brief Get query parameters
 * @param implementation http_request_t instance
 * @param result Query parameters. Delete with free function
 * @param size Size of query parameters
 * @return Error if occurred
 */
web_framework_exception_t wf_get_query_parameters(http_request_t implementation, query_parameter_t** result, size_t* size);

/**
 * @brief Get HTTP version
 * @param implementation http_request_t instance
 * @param version String representation of HTTP version. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_http_version(http_request_t implementation, web_framework_string_t* version);

/**
 * @brief Get HTTP headers
 * @param implementation http_request_t instance
 * @param result HTTP headers. Delete with free function
 * @param size Size of HTTP headers
 * @return Error if occurred
 */
web_framework_exception_t wf_get_http_headers(http_request_t implementation, http_header_t** result, size_t* size);

/**
 * @brief Get single HTTP header
 * @param implementation http_request_t instance
 * @param headerName HTTP header name
 * @param result HTTP header value
 * @return Error if occurred
 */
web_framework_exception_t wf_get_http_header(http_request_t implementation, const char* header_name, const char** result);

/**
 * @brief Get body of HTTP request
 * @param implementation http_request_t instance
 * @param body HTTP body
 * @param bodySize HTTP body size
 * @return Error if occurred
 */
web_framework_exception_t wf_get_http_body(http_request_t implementation, const char** body, size_t* body_size);

/**
 * @brief Set HTTP session attribute
 * @param implementation http_request_t instance
 * @param name Attribute name
 * @param value Attribute value
 * @return Error if occurred
 */
web_framework_exception_t wf_set_attribute(http_request_t implementation, const char* name, const char* value);

/**
 * @brief Get HTTP session attribute
 * @param implementation http_request_t instance
 * @param name Attribute name
 * @param result Attribute value. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_attribute(http_request_t implementation, const char* name, web_framework_string_t* result);

/**
 * @brief Delete session
 * @param implementation http_request_t instance 
 * @return Error if occurred
 */
web_framework_exception_t wf_delete_session(http_request_t implementation);

/**
 * @brief Remove HTTP session attribute
 * @param implementation http_request_t instance
 * @param name Attribute name
 * @return Error if occurred
 */
web_framework_exception_t wf_remove_attribute(http_request_t implementation, const char* name);

/**
 * @brief Get cookies
 * @param implementation http_request_t instance
 * @param result Cookies. Delete with free function
 * @param size Size of Cookies
 * @return Error if occurred
 */
web_framework_exception_t wf_get_cookies(http_request_t implementation, cookie_t** result, size_t* size);

/**
 * @brief Get multiparts
 * @param implementation http_request_t instance 
 * @param result Multiparts. Delete with free function
 * @param size Size of multiparts
 * @return Error if occurred
 */
web_framework_exception_t wf_get_multiparts(http_request_t implementation, multipart_t** result, size_t* size);

/**
 * @brief Get current large data
 * @param implementation http_request_t instance
 * @param result LargeData
 * @return Error if occurred
 */
web_framework_exception_t wf_get_large_data(http_request_t implementation, const large_data_t** result);

/**
 * @brief Send file
 * @param implementation http_request_t instance
 * @param filePath Relative path to file from assets directory
 * @param response http_response_t instance
 * @param variables Variables for processing if file is .wfdp
 * @param variablesSize Size of variables
 * @param isBinary Is binary file
 * @param fileName Optional parameter. Custom name
 * @return Error if occurred
 */
web_framework_exception_t wf_send_asset_file(http_request_t implementation, const char* file_path, http_response_t response, const dynamic_pages_variable_t* variables, size_t variables_size, bool is_binary, const char* file_name);

/**
 * @brief Send static file
 * @param implementation http_request_t instance
 * @param filePath Relative path to file from assets directory
 * @param response http_response_t instance
 * @param isBinary Is binary file
 * @param fileName Optional parameter. Custom name
 * @return Error if occurred
 */
web_framework_exception_t wf_send_static_file(http_request_t implementation, const char* file_path, http_response_t response, bool is_binary, const char* file_name);

/**
 * @brief Send .wfdp file
 * @param implementation http_request_t instance
 * @param filePath Relative path to file from assets directory
 * @param response http_response_t instance
 * @param variables Variables for processing .wfdp file
 * @param variablesSize Size of variables
 * @param isBinary Is binary file
 * @param fileName Optional parameter. Custom name
 * @return Error if occurred
 */
web_framework_exception_t wf_send_dynamic_file(http_request_t implementation, const char* file_path, http_response_t response, const dynamic_pages_variable_t* variables, size_t variables_size, bool is_binary, const char* file_name);

/**
 * @brief Send file with specific chunk size
 * @param implementation http_request_t instance
 * @param filePath Relative path to file from assets directory
 * @param response http_response_t instance
 * @param fileName Name of file
 * @param chunkSize Chunk size
 * @return Error if occurred
 */
web_framework_exception_t wf_stream_file(http_request_t implementation, const char* file_path, http_response_t response, const char* file_name, size_t chunk_size);

/**
 * @brief Register function for processing .wfdp files
 * @param implementation http_request_t instance
 * @param functionName Function name
 * @param function Callback called in processing .wfdp files
 * @param deleter Deleter for values from function
 * @return 
 */
web_framework_exception_t wf_register_wfdp_function(http_request_t implementation, const char* function_name, const char* (*function)(const char** arguments, size_t arguments_number), void(*deleter)(char* result));

/**
 * @brief Unregister function for processing .wfdp files
 * @param implementation http_request_t instance
 * @param functionName Function name
 * @return Error if occurred
 */
web_framework_exception_t wf_unregister_wfdp_function(http_request_t implementation, const char* function_name);

/**
 * @brief Check is WFDP function registered
 * @param implementation http_request_t instance
 * @param functionName Function name
 * @param result Is function registered
 * @return Error if occurred
 */
web_framework_exception_t wf_is_wfdp_function_registered(http_request_t implementation, const char* function_name, bool* result);

/**
 * @brief Get HTTP body as Json
 * @param implementation http_request_t instance
 * @param parser Json data
 * @return Error if occurred
 */
web_framework_exception_t wf_get_request_json(http_request_t implementation, json_parser_t* parser);

/**
 * @brief Get HTTP chunks
 * @param implementation http_request_t instance
 * @param result Chunks
 * @param size Size of chunks
 * @return Error if occurred
 */
web_framework_exception_t wf_get_chunks(http_request_t implementation, http_chunk_t** result, size_t* size);

/**
 * @brief Get file content
 * @param implementation http_request_t instance
 * @param filePath Path to asset file from assets folder
 * @param result File content. Delete with free function
 * @param size File content size
 * @return Error if occurred
 */
web_framework_exception_t wf_get_file(http_request_t implementation, const char* file_path, const char** result, size_t* size);

/**
 * @brief Process static files like .md
 * @param implementation http_request_t instance
 * @param fileData Static file content
 * @param size fileData size
 * @param fileExtension Supported processing extension
 * @param result Processed data
 * @param resultSize Processed data size
 * @return 
 */
web_framework_exception_t wf_process_static_file(http_request_t implementation, const char* file_data, size_t size, const char* file_extension, const char** result, size_t* result_size);

/**
 * @brief Process .wfdp files
 * @param implementation http_request_t instance
 * @param fileData WFDP file content
 * @param variables Variables for processing .wfdp file
 * @param variablesSize Size of variables
 * @param result Processed data
 * @param resultSize Processed data size
 * @return 
 */
web_framework_exception_t wf_process_dynamic_file(http_request_t implementation, const char* file_data, size_t size, const dynamic_pages_variable_t* variables, size_t variables_size, const char** result, size_t* result_size);

/**
 * @brief Get raw HTTP request
 * @param implementation http_request_t instance
 * @param rawRequest Raw HTTP request
 * @return Error if occurred
 */
web_framework_exception_t wf_get_raw_request(http_request_t implementation, const char** raw_request);

/**
 * @brief Get client IP v4
 * @param implementation http_request_t instance
 * @param ip IP. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_client_ip_v4(http_request_t implementation, web_framework_string_t* ip);

/**
 * @brief Get server IP v4
 * @param implementation http_request_t instance
 * @param ip IP. Delete with deleteWebFrameworkString function
 * @return Error if occurred
 */
web_framework_exception_t wf_get_server_ip_v4(http_request_t implementation, web_framework_string_t* ip);

/**
 * @brief Get client port
 * @param implementation http_request_t instance
 * @param port Client port
 * @return Error if occurred
 */
web_framework_exception_t wf_get_client_port(http_request_t implementation, uint16_t* port);

/**
 * @brief Get server port
 * @param implementation http_request_t instance
 * @param port Server port
 * @return Error if occurred
 */
web_framework_exception_t wf_get_server_port(http_request_t implementation, uint16_t* port);

/**
 * @brief Tries to get database or create if can't get
 * @param implementation http_request_t instance
 * @param databaseName database_t name
 * @param result database_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_or_create_database_request(http_request_t implementation, const char* database_name, database_t* result);

/**
 * @brief Get database
 * @param implementation http_request_t instance
 * @param databaseName database_t name
 * @param result database_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_database_request(http_request_t implementation, const char* database_name, database_t* result);

/**
 * @brief Tries to get table from database or create if can't get
 * @param implementation http_request_t instance
 * @param databaseName database_t name
 * @param tableName Name of table
 * @param createTableQuery Create table query if can't get table
 * @param result table_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_or_create_table_request(http_request_t implementation, const char* database_name, const char* table_name, const char* create_table_query, table_t* result);

/**
 * @brief Get table from database
 * @param implementation http_request_t instance
 * @param databaseName database_t name
 * @param tableName Name of table
 * @param result table_t instance
 * @return Error if occurred
 */
web_framework_exception_t wf_get_table_request(http_request_t implementation, const char* database_name, const char* table_name, table_t* result);

/**
 * @brief Get route parameter as /page/{}
 * @param implementation http_request_t instance
 * @param routeParameterName Route parameter name
 * @param result Route parameter value
 * @return Error if occurred
 */
web_framework_exception_t wf_get_route_integer_parameter(http_request_t implementation, const char* route_parameter_name, int64_t* result);

/**
 * @brief Get route parameter as /page/{}
 * @param implementation http_request_t instance
 * @param routeParameterName Route parameter name
 * @param result Route parameter value
 * @return Error if occurred
 */
web_framework_exception_t wf_get_route_double_parameter(http_request_t implementation, const char* route_parameter_name, double* result);

/**
 * @brief Get route parameter as /page/{}
 * @param implementation http_request_t instance
 * @param routeParameterName Route parameter name
 * @param result Route parameter value
 * @return Error if occurred
 */
web_framework_exception_t wf_get_route_string_parameter(http_request_t implementation, const char* route_parameter_name, const char** result);

/**
 * @brief Send generated chunks
 * @param implementation http_request_t instance
 * @param response http_response_t instance
 * @param chunkGenerator Function that generates chunks
 * @param data Custom data
 * @return Error if occurred
 */
web_framework_exception_t wf_send_chunks(http_request_t implementation, http_response_t response, const char* (*chunk_generator)(void* data, size_t* size), void* data);

/**
 * @brief Send file as chunks
 * @param implementation http_request_t instance
 * @param response http_response_t instance
 * @param fileName File name
 * @param chunkGenerator Function that generate chunks
 * @param data Custom data
 * @return Error if occurred
 */
web_framework_exception_t wf_send_file_chunks(http_request_t implementation, http_response_t response, const char* file_name, const char* (*chunk_generator)(void* data, size_t* size), void* data);

/**
 * @brief Return from function with message and HTTP response code. Used for throwing errors
 * @param implementation http_request_t instance
 * @param exceptionMessage Error message
 * @param responseCode HTTP response code
 * @param logCategory Log category for logging
 * @param exceptionHash Same errors must have same hash
 * @return 
 */
web_framework_exception_t wf_throw_web_framework_exception(http_request_t implementation, const char* exception_message, response_codes_t response_code, const char* log_category, size_t exception_hash);
