#include "CXXExecutor.h"

#include <TaskBroker/RabbitMqTaskSerializer.hpp>

constexpr std::string_view message = "CXX message";

class CXXTaskSerializer : public framework::task_broker::RabbitMqTaskSerializer<>
{
private:
	std::string suffix;

protected:
	framework::JsonObject serializeArguments() const override
	{
		constexpr std::string_view fileName = "cxx_cxx_{}.txt";

		framework::JsonObject result;

		result["fileName"] = std::vformat(fileName, std::make_format_args(suffix));
		result["message"] = message;

		return result;
	}

	std::string_view getTaskExecutorName() const override
	{
		return "CXXTestTaskExecutor";
	}

	std::string_view getQueueName() const override
	{
		return "cxx_queue";
	}

public:
	CXXTaskSerializer(std::string_view suffix) :
		suffix(suffix)
	{

	}

	~CXXTaskSerializer() = default;
};

class CCTaskSerializer : public framework::task_broker::RabbitMqTaskSerializer<framework::task_broker::CCApi>
{
private:
	std::string suffix;

protected:
	framework::JsonObject serializeArguments() const override
	{
		constexpr std::string_view fileName = "cxx_cc_{}.txt";

		framework::JsonObject result;

		result["fileName"] = std::vformat(fileName, std::make_format_args(suffix));
		result["message"] = message;

		return result;
	}

	std::string_view getTaskExecutorName() const override
	{
		return "c_test_task_executor";
	}

	std::string_view getQueueName() const override
	{
		return "cc_queue";
	}

public:
	CCTaskSerializer(std::string_view suffix) :
		suffix(suffix)
	{

	}

	~CCTaskSerializer() = default;
};

class PythonTaskSerializer : public framework::task_broker::RabbitMqTaskSerializer<framework::task_broker::PythonApi>
{
private:
	std::string suffix;

protected:
	framework::JsonObject serializeArguments() const override
	{
		constexpr std::string_view fileName = "cxx_python_{}.txt";

		framework::JsonObject result;

		result["fileName"] = std::vformat(fileName, std::make_format_args(suffix));
		result["message"] = message;

		return result;
	}

	std::string_view getTaskExecutorName() const override
	{
		return "PythonTestTaskExecutor";
	}

	std::string_view getQueueName() const override
	{
		return "python_queue";
	}

public:
	PythonTaskSerializer(std::string_view suffix) :
		suffix(suffix)
	{

	}

	~PythonTaskSerializer() = default;
};

class CSharpTaskSerializer : public framework::task_broker::RabbitMqTaskSerializer<framework::task_broker::CSharpApi>
{
private:
	std::string suffix;

protected:
	framework::JsonObject serializeArguments() const override
	{
		constexpr std::string_view fileName = "cxx_csharp_{}.txt";

		framework::JsonObject result;

		result["fileName"] = std::vformat(fileName, std::make_format_args(suffix));
		result["message"] = message;

		return result;
	}

	std::string_view getTaskExecutorName() const override
	{
		return "CSharpTestTaskExecutor";
	}

	std::string_view getQueueName() const override
	{
		return "csharp_queue";
	}

public:
	CSharpTaskSerializer(std::string_view suffix) :
		suffix(suffix)
	{

	}

	~CSharpTaskSerializer() = default;
};

void CXXExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	std::string taskBroker = request.getJson().get<std::string>("taskBroker");

	if (taskBroker == framework::task_broker::Internal::taskBrokerName)
	{
		constexpr std::string_view suffix = "internal";

		request.enqueueTask<framework::task_broker::Internal, CXXTaskSerializer>(suffix);
		request.enqueueTask<framework::task_broker::Internal, CCTaskSerializer>(suffix);
		request.enqueueTask<framework::task_broker::Internal, PythonTaskSerializer>(suffix);
		request.enqueueTask<framework::task_broker::Internal, CSharpTaskSerializer>(suffix);
	}
	else if (taskBroker == framework::task_broker::RabbitMq::taskBrokerName)
	{
		constexpr std::string_view suffix = "rabbitmq";

		request.enqueueTask<framework::task_broker::RabbitMq, CXXTaskSerializer>(suffix);
		request.enqueueTask<framework::task_broker::RabbitMq, CCTaskSerializer>(suffix);
		request.enqueueTask<framework::task_broker::RabbitMq, PythonTaskSerializer>(suffix);
		request.enqueueTask<framework::task_broker::RabbitMq, CSharpTaskSerializer>(suffix);
	}
	else
	{
		request.throwException(std::format("Wrong taskBroker name: {}", taskBroker), framework::ResponseCodes::badRequest);
	}

	response.setBody("Accepted");
	response.setResponseCode(framework::ResponseCodes::accepted);
}

DEFINE_EXECUTOR(CXXExecutor);
