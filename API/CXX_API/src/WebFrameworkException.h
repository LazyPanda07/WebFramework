#pragma once

#include <stdexcept>

#include "DLLHandler.h"

namespace framework
{
	class WebFrameworkException : public std::exception
	{
	private:
		std::shared_ptr<void> implementation;

	public:
		WebFrameworkException(void* implementation);

		const char* what() const noexcept override;

		~WebFrameworkException() = default;
	};

	inline WebFrameworkException::WebFrameworkException(void* implementation) :
		implementation
		(
			std::shared_ptr<void>
			(
				implementation,
				[this](void* block)
				{
					DLLHandler::getInstance().free(block);
				}
			)
		)
	{

	}

	inline const char* WebFrameworkException::what() const noexcept
	{
		using getErrorMessage = const char* (*)(void* implementation);

		return DLLHandler::getInstance().CALL_FUNCTION(getErrorMessage, implementation.get());
	}
}
