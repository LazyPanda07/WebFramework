#include "CXXExecutor.h"

constexpr std::string_view message = "CXX message";

class CXXTaskSerializer : public framework::task_broker::TaskSerializer<>
{
protected:
	framework::JsonObject serializeArguments() const
	{
		constexpr std::string_view fileName = "cxx_cxx.txt";

		framework::JsonObject result;

		result["fileName"] = fileName;
		result["message"] = message;

		return result;
	}

	std::string_view getTaskName() const
	{
		return "CXXTestTaskExecutor";
	}

public:
	CXXTaskSerializer() = default;

	~CXXTaskSerializer() = default;
};

class CCTaskSerializer : public framework::task_broker::TaskSerializer<framework::task_broker::CCApi>
{
protected:
	framework::JsonObject serializeArguments() const
	{
		constexpr std::string_view fileName = "cxx_cc.txt";

		framework::JsonObject result;

		result["fileName"] = fileName;
		result["message"] = message;

		return result;
	}

	std::string_view getTaskName() const
	{
		return "c_test_task_executor";
	}

public:
	CCTaskSerializer() = default;

	~CCTaskSerializer() = default;
};

class PythonTaskSerializer : public framework::task_broker::TaskSerializer<framework::task_broker::PythonApi>
{
protected:
	framework::JsonObject serializeArguments() const
	{
		constexpr std::string_view fileName = "cxx_python.txt";

		framework::JsonObject result;

		result["fileName"] = fileName;
		result["message"] = message;

		return result;
	}

	std::string_view getTaskName() const
	{
		return "PythonTestTaskExecutor";
	}

public:
	PythonTaskSerializer() = default;

	~PythonTaskSerializer() = default;
};

class CSharpTaskSerializer : public framework::task_broker::TaskSerializer<framework::task_broker::CSharpApi>
{
protected:
	framework::JsonObject serializeArguments() const
	{
		constexpr std::string_view fileName = "cxx_csharp.txt";

		framework::JsonObject result;

		result["fileName"] = fileName;
		result["message"] = message;

		return result;
	}

	std::string_view getTaskName() const
	{
		return "CSharpTestTaskExecutor";
	}

public:
	CSharpTaskSerializer() = default;

	~CSharpTaskSerializer() = default;
};

void CXXExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	request.enqueueTask<framework::task_broker::Internal, CXXTaskSerializer>();
	request.enqueueTask<framework::task_broker::Internal, CCTaskSerializer>();
	request.enqueueTask<framework::task_broker::Internal, PythonTaskSerializer>();
	request.enqueueTask<framework::task_broker::Internal, CSharpTaskSerializer>();

	response.setBody("Accepted");
	response.setResponseCode(framework::ResponseCodes::accepted);
}

DEFINE_EXECUTOR(CXXExecutor);
