#include "gtest/gtest.h"

#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "JSONParser.h"

#include "utilities.h"

TEST(Database, Create)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().postRequest().build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok);
}

TEST(Database, Insert)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().putRequest().build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok);
}

TEST(Database, Select)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().build();
	std::string response;

	stream << request;

	stream >> response;

	web::HTTPParser parser(response);

	std::cout << parser.getBody() << std::endl;

	ASSERT_EQ(parser.getResponseCode(), web::responseCodes::ok);
}

TEST(Database, Update)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().build();
	std::string response;

	stream << request;

	stream >> response;

	web::HTTPParser parser(response);

	std::cout << parser.getBody() << std::endl;

	ASSERT_EQ(parser.getResponseCode(), web::responseCodes::ok);
}

TEST(Database, Delete)
{
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HTTPBuilder().getRequest().build();
	std::string response;

	stream << request;

	stream >> response;

	ASSERT_EQ(web::HTTPParser(response).getResponseCode(), web::responseCodes::ok);
}
