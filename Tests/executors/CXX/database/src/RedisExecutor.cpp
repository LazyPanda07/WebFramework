#include "RedisExecutor.h"

#include <Utility/WebFrameworkUtility.hpp>

void RedisExecutor::init(const framework::utility::ExecutorSettings& settings)
{
#ifdef WITHOUT_REDIS_TESTS
	return;
#endif

	settings.getOrCreateDatabase<framework::RedisDatabase>("127.0.0.1:10010:password").getOrCreateTable("", "");
}

void RedisExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Table connect = request.getTable<framework::RedisDatabase>("127.0.0.1:10010:password", "");
	framework::JsonBuilder result;
	
	{
		framework::SqlResult temp = connect.execute("GET", framework::utility::database::makeSQLValues("string"));

		result["string"] = temp[0].begin()->second.get<std::string>();
	}

	{
		framework::SqlResult temp = connect.execute("GET", framework::utility::database::makeSQLValues("int"));

		result["int"] = temp[0].begin()->second.get<int64_t>();
	}

	{
		framework::SqlResult temp = connect.execute("GET", framework::utility::database::makeSQLValues("double"));

		result["double"] = temp[0].begin()->second.get<double>();
	}

	{
		framework::SqlResult temp = connect.execute("GET", framework::utility::database::makeSQLValues("bool"));

		result["bool"] = temp[0].begin()->second.get<bool>();
	}

	response.setBody(result);
}

void RedisExecutor::doPut(framework::HttpRequest& request, framework::HttpResponse& response)
{
	framework::Table connect = request.getTable<framework::RedisDatabase>("127.0.0.1:10010:password", "");
	
	connect.execute("SET", framework::utility::database::makeSQLValues("string", "qwe"));
	connect.execute("SET", framework::utility::database::makeSQLValues("int", 5));
	connect.execute("SET", framework::utility::database::makeSQLValues("double", 2.3));
	connect.execute("SET", framework::utility::database::makeSQLValues("bool", true));

	response.setResponseCode(framework::ResponseCodes::created);
}

DEFINE_EXECUTOR(RedisExecutor)
