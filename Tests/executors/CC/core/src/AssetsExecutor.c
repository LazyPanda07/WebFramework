#include <executors/executor.h>

#include <signal.h>

typedef enum request_type
{
	print,
	custom,
	none
} request_type_t;

DEFINE_DEFAULT_EXECUTOR(AssetsExecutor, STATELESS_EXECUTOR)

static char* custom_function(json_object_t arguments);

static void deleter(char* ptr);

static request_type_t get_type(query_parameter_t* parameters);

DEFINE_EXECUTOR_INIT(AssetsExecutor)
{
	wf_register_dynamic_function_executor_settings(settings, "customFunction", custom_function, deleter);
}

DEFINE_EXECUTOR_METHOD(AssetsExecutor, GET_METHOD, request, response)
{
	json_parser_t parser;
	const char* fileName;
	const char extension[] = "wfdp";
	query_parameter_t* queryParameters;
	size_t queryParametersSize;
	json_object_t arguments;

	wf_get_request_json(request, &parser);
	wf_get_query_parameters(request, &queryParameters, &queryParametersSize);
	wf_create_json_object(&arguments);

	wf_get_json_parser_string(parser, "fileName", true, &fileName);

	if (queryParametersSize)
	{
		switch (get_type(queryParameters))
		{
		case print:
		{
			{
				json_object_t print;

				wf_assign_or_get_json_object(&arguments, "@print", &print);
				wf_assign_or_get_json_object(&print, "data", NULL);
			}

			json_object_t print;
			json_object_t data;

			wf_assign_or_get_json_object(&arguments, "@print", &print);
			wf_assign_or_get_json_object(&print, "data", &data);

			wf_set_json_object_string(&data, queryParameters[0].value);
		}

		break;

		case custom:
		{
			{
				json_object_t custom_function_data;

				wf_assign_or_get_json_object(&arguments, "@customFunction", &custom_function_data);
				wf_assign_or_get_json_object(&custom_function_data, "data", NULL);
			}

			json_object_t custom_function_data;
			json_object_t data;
			
			wf_assign_or_get_json_object(&arguments, "@customFunction", &custom_function_data);
			wf_assign_or_get_json_object(&custom_function_data, "data", &data);

			for (size_t i = 0; i < queryParametersSize; i++)
			{
				json_object_t temp;

				wf_emplace_json_object(&data, &temp);

				wf_set_json_object_integer(&temp, atoll(queryParameters[i].value));
			}
		}

		break;

		default:
			break;
		}
	}

	char fullName[1024];

	memset(fullName, 0, sizeof(fullName));

	snprintf(fullName, sizeof(fullName), "%s.%s", fileName, extension);

	wf_send_dynamic_file(request, fullName, response, &arguments, false, "");

	wf_delete_json_object(&arguments);
}

DEFINE_EXECUTOR_METHOD(AssetsExecutor, DELETE_METHOD, request, response)
{
	wf_unregister_dynamic_function(request, "customFunction");
}

char* custom_function(json_object_t arguments)
{
	json_object_t custom_function_data;
	json_object_t data;
	int64_t first;
	int64_t second;
	int64_t third;

	wf_assign_or_get_json_object(&arguments, "@customFunction", &custom_function_data);
	wf_assign_or_get_json_object(&custom_function_data, "data", &data);

	{
		json_object_t temp;

		wf_index_access_json_object_array(&data, 0, &temp);

		wf_get_json_object_integer(&temp, &first);
	}

	{
		json_object_t temp;

		wf_index_access_json_object_array(&data, 1, &temp);

		wf_get_json_object_integer(&temp, &second);
	}

	{
		json_object_t temp;

		wf_index_access_json_object_array(&data, 2, &temp);

		wf_get_json_object_integer(&temp, &third);
	}

	size_t buffer_size = 1024;
	char* buffer = calloc(buffer_size, sizeof(char));

	snprintf(buffer, buffer_size, "Data: %lld %lld %lld", first, second, third);

	return buffer;
}

void deleter(char* ptr)
{
	free(ptr);
}

request_type_t get_type(query_parameter_t* parameters)
{
	if (!strcmp(parameters[0].key, "data"))
	{
		return print;
	}
	else if (!strcmp(parameters[0].key, "first"))
	{
		return custom;
	}

	return none;
}

DEFINE_INITIALIZE_WEB_FRAMEWORK()
