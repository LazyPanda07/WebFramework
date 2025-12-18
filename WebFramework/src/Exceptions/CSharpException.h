#pragma once

#include <Exceptions/BaseWebFrameworkException.h>

#include <HttpUtility.h>

#ifdef __WITH_DOT_NET_EXECUTORS__

namespace framework::exceptions
{
	class CSharpException : public BaseWebFrameworkException
	{
	private:
		web::ResponseCodes responseCode;
		std::string logCategory;

	public:
		CSharpException(std::string_view errorMessage, web::ResponseCodes responseCode, std::string_view logCategory);

		web::ResponseCodes getResponseCode() const;

		std::string_view getLogCategory() const;

		~CSharpException() = default;
	};
}

#endif
