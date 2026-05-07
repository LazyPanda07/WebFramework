#include <gtest/gtest.h>
#include <openssl/ssl.h>

#include <HttpBuilder.h>
#include <HttpParser.h>
#include <Base64.h>
#include <WebSocket/WsNetwork.h>

#include "utilities.h"

class WebSocketEcho : public testing::Test
{
protected:
	static inline std::unique_ptr<streams::IOSocketStream> webSocketStream;

protected:
	static std::string getRandomBase64Bytes()
	{
		std::string result(16, '\0');

		for (size_t i = 0; i < result.size(); i++)
		{
			result[i] = static_cast<uint8_t>(rand() % 128);
		}

		return result;
	};

	static std::string generateRandomText(size_t size)
	{
		std::string result;

		result.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			result += rand() % 26 + 65;
		}

		return utility::conversion::encodeBase64(result);
	};
};

TEST_F(WebSocketEcho, Upgrade)
{
	std::string randomBytes = WebSocketEcho::getRandomBase64Bytes();
	streams::IOSocketStream stream = utility::createSocketStream();
	std::string request = web::HttpBuilder()
		.getRequest()
		.headers
		(
			"Host", "127.0.0.1",
			"Upgrade", "websocket",
			"Connection", "Upgrade",
			"Sec-WebSocket-Key", randomBytes,
			"Sec-WebSocket-Version", 13 // ignored
		)
		.build();
	std::string response;

	stream << request;

	stream >> response;

	web::HttpParser parser(response);

	ASSERT_EQ(parser.getResponseCode(), web::ResponseCodes::switchingProtocols) << parser;

	{
		constexpr std::string_view guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

		std::string temp = std::format("{}{}", randomBytes, guid);
		std::vector<uint8_t> output(SHA_DIGEST_LENGTH);

		SHA1(reinterpret_cast<const uint8_t*>(temp.data()), temp.size(), output.data());

		ASSERT_EQ(utility::conversion::encodeBase64(output), parser.getHeaders().at("Sec-WebSocket-Accept"));
	}

	utility::createWebSocketStream(stream);

	WebSocketEcho::webSocketStream = std::make_unique<streams::IOSocketStream>(std::move(stream));
}

TEST_F(WebSocketEcho, Echo)
{
	std::vector<size_t> sizes =
	{
		50,
		40'000,
		120'000
	};

	for (size_t size : sizes)
	{
		std::string randomText(WebSocketEcho::generateRandomText(size));
		std::string response;

		(*WebSocketEcho::webSocketStream) << randomText;

		(*WebSocketEcho::webSocketStream) >> response;

		ASSERT_EQ(randomText, response);
	}

	web::web_socket::Frame sendFrame(true, web::web_socket::Frame::OpcodeType::text, "Hello, World!", web::web_socket::Frame::generateMask());
	std::vector<web::web_socket::Frame> echoFrames;

	(*WebSocketEcho::webSocketStream) << sendFrame;

	(*WebSocketEcho::webSocketStream) >> echoFrames;

	ASSERT_EQ(echoFrames.size(), 1);

	web::web_socket::Frame& echoFrame = echoFrames.front();

	ASSERT_EQ(sendFrame.getUnmaskedPayload(), echoFrame.getPayload());
	ASSERT_EQ(sendFrame.getFrameOpcode(), echoFrame.getFrameOpcode());
}
