#pragma once

#include <span>

#include "../DLLHandler.hpp"

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
			std::span<T> getPayload() const;

			Type getType() const;

			operator bool() const;

			~Frame() = default;
		};

	public:
		WebSocketExecutor() = default;

		virtual void send(const Frame& frame) = 0;

		virtual void onReceive(const Frame& frame) = 0;

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
	std::span<T> WebSocketExecutor::Frame::getPayload() const
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
WEB_FRAMEWORK_FUNCTIONS_API inline void webFrameworkCXXWebSocketExecutorSend(void* implementation, void* frame)
{
	framework::WebSocketExecutor::Frame frameWrapper(frame);

	static_cast<framework::WebSocketExecutor*>(implementation)->send(frameWrapper);
}

WEB_FRAMEWORK_FUNCTIONS_API inline void webFrameworkCXXWebSocketExecutorOnReceive(void* implementation, void* frame)
{
	framework::WebSocketExecutor::Frame frameWrapper(frame);

	static_cast<framework::WebSocketExecutor*>(implementation)->onReceive(frameWrapper);
}

WEB_FRAMEWORK_FUNCTIONS_API inline void webFrameworkCXXDeleteWebSocketExecutor(void* implementation)
{
	delete static_cast<framework::WebSocketExecutor*>(implementation);
}
#pragma endregion
