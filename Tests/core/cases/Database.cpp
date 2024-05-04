#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"

#include "utilities.h"

TEST(Database, Create)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().postRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok) << response;
}

TEST(Database, Insert)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().putRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok) << response;
}

TEST(Database, Select)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getJSON().getArray("data").size(), 11) << response;
}

TEST(Database, Update)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().patchRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getJSON().getArray("data").size(), 1) << response;
}

TEST(Database, Delete)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().parameters("database").build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok) << response;
}
