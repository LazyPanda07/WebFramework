#pragma once

#include <span>
#include <variant>

#include "DLLHandler.hpp"

namespace framework
{
	class WebSocketExecutor
	{
	public:
		class Frame
		{
		public:
			enum class Type
			{
				text = 0x1,
				binary = 0x2
			};

		private:
			void* implementation;

		public:
			Frame(void* implementation);

			template<typename T = char>
			std::span<T> getPayload() const requires(std::same_as<T, char> || std::same_as<T, uint8_t>);

			Type getType() const;

			explicit operator bool() const;

			~Frame() = default;
		};

	public:
		WebSocketExecutor() = default;

		virtual std::variant<std::string, std::vector<uint8_t>> onReceive(const Frame& frame) = 0;

		virtual ~WebSocketExecutor() = default;
	};
}

namespace framework
{
	WebSocketExecutor::Frame::Frame(void* implementation) :
		implementation(implementation)
	{

	}

	template<typename T>
	std::span<T> WebSocketExecutor::Frame::getPayload() const requires(std::same_as<T, char> || std::same_as<T, uint8_t>)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(getFramePayload, char*, uint64_t*, void** exception);
		void* exception = nullptr;

		uint64_t size = 0;
		
		char* ptr = utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getFramePayload, &size, &exception);

		return std::span<T>(reinterpret_cast<T>(ptr), size);
	}

	WebSocketExecutor::Frame::Type WebSocketExecutor::Frame::getType() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(getFrameType, int, void** exception);
		void* exception = nullptr;

		return static_cast<Frame::Type>(utility::DllHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(getFrameType, &exception));
	}

	WebSocketExecutor::Frame::operator bool() const
	{
		return static_cast<bool>(implementation);
	}
}

/**
* Macro for each WebSocketExecutor subclass
* Used for loading function that creates WebSocketExecutor subclass
*/
#define DEFINE_WEB_SOCKET_EXECUTOR(subclassName) WEB_FRAMEWORK_FUNCTIONS_API void* create##subclassName##WebSocketCXXInstance()	\
{	\
	return new subclassName();	\
}

#pragma region ExportFunctions
WEB_FRAMEWORK_FUNCTIONS_API inline void webFrameworkCXXWebSocketExecutorOnReceive(void* implementation, void* frame, void(*sendData)(const uint8_t* data, size_t size, int32_t type))
{
	framework::WebSocketExecutor::Frame frameWrapper(frame);

	std::variant<std::string, std::vector<uint8_t>> data = static_cast<framework::WebSocketExecutor*>(implementation)->onReceive(frameWrapper);
	const uint8_t* ptr = nullptr;
	size_t size = 0;
	framework::WebSocketExecutor::Frame::Type type = framework::WebSocketExecutor::Frame::Type::binary;

	if (std::holds_alternative<std::string>(data))
	{
		const std::string& temp = std::get<std::string>(data);

		ptr = reinterpret_cast<const uint8_t*>(temp.data());
		size = temp.size();
		type = framework::WebSocketExecutor::Frame::Type::text;
	}
	else if (std::holds_alternative<std::vector<uint8_t>>(data))
	{
		const std::vector<uint8_t>& temp = std::get<std::vector<uint8_t>>(data);

		ptr = reinterpret_cast<const uint8_t*>(temp.data());
		size = temp.size();
		type = framework::WebSocketExecutor::Frame::Type::binary;
	}
	else
	{
		throw std::runtime_error("Wrong frame send variant type");
	}

	sendData(ptr, size, static_cast<int32_t>(type));
}

WEB_FRAMEWORK_FUNCTIONS_API inline void webFrameworkCXXDeleteWebSocketExecutor(void* implementation)
{
	delete static_cast<framework::WebSocketExecutor*>(implementation);
}
#pragma endregion
