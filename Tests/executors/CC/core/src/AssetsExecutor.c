#include <executors/executor.h>

#include <inttypes.h>

typedef enum request_type
{
	print,
	custom,
	none
} request_type_t;

DEFINE_DEFAULT_EXECUTOR(AssetsExecutor, STATELESS_EXECUTOR)

static int cmp(const void* a, const void* b);

static char* custom_function(json_object_t arguments);

static void deleter(char* ptr);

static request_type_t get_type(query_parameter_t* parameters, size_t size);

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
		switch (get_type(queryParameters, queryParametersSize))
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

int cmp(const void* a, const void* b)
{
	int64_t x = *(const int64_t*)a;
	int64_t y = *(const int64_t*)b;

	if (x < y) return -1;
	if (x > y) return 1;

	return 0;
}

char* custom_function(json_object_t arguments)
{
	json_object_t custom_function_data;
	json_object_t data;
	int64_t array[3];

	wf_assign_or_get_json_object(&arguments, "@customFunction", &custom_function_data);
	wf_assign_or_get_json_object(&custom_function_data, "data", &data);

	{
		json_object_t temp;

		wf_index_access_json_object_array(&data, 0, &temp);

		wf_get_json_object_integer(&temp, &array[0]);
	}

	{
		json_object_t temp;

		wf_index_access_json_object_array(&data, 1, &temp);

		wf_get_json_object_integer(&temp, &array[1]);
	}

	{
		json_object_t temp;

		wf_index_access_json_object_array(&data, 2, &temp);

		wf_get_json_object_integer(&temp, &array[2]);
	}

	size_t buffer_size = 1024;
	char* buffer = calloc(buffer_size, sizeof(char));

	qsort(array, sizeof(array) / sizeof(int64_t), sizeof(int64_t), cmp);

	snprintf(buffer, buffer_size, "Data: %" PRId64 " %" PRId64 " %" PRId64, array[0], array[1], array[2]);

	return buffer;
}

void deleter(char* ptr)
{
	free(ptr);
}

request_type_t get_type(query_parameter_t* parameters, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (!strcmp(parameters[i].key, "data"))
		{
			return print;
		}
		else if (!strcmp(parameters[i].key, "first"))
		{
			return custom;
		}
	}

	return none;
}

DEFINE_INITIALIZE_WEB_FRAMEWORK()
