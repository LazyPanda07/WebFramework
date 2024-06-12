#pragma once

#include <stdexcept>

#include "DLLHandler.h"

namespace framework_api
{
	class WebFrameworkException : public std::exception
	{
	private:
		std::shared_ptr<DLLHandler> handler;
		std::shared_ptr<void> implementation;

	public:
		WebFrameworkException(std::shared_ptr<DLLHandler> handler, void* implementation);

		const char* what() const override;

		~WebFrameworkException() = default;
	};

	WebFrameworkException::WebFrameworkException(std::shared_ptr<DLLHandler> handler, void* implementation) :
		handler(handler),
		implementation
		(
			std::shared_ptr<void>
			(
				implementation,
				[this](void* block)
				{
					this->handler->free(block);
				}
			)
		)
	{

	}

	const char* WebFrameworkException::what() const
	{
		using getErrorMessage = const char* (*)(void** exception);

		return handler->CALL_FUNCTION(getErrorMessage, &implementation);
	}
}
