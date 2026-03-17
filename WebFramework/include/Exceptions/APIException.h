#pragma once

#include <Exceptions/BaseWebFrameworkException.h>

namespace framework::exceptions
{
	class APIException : public BaseWebFrameworkException
	{
	private:
		std::string logCategory;
		int64_t responseCode;
		size_t exceptionClassHash;

	public:
		APIException(std::string_view errorMessage, int64_t responseCode, std::string_view logCategory, size_t exceptionClassHash);

		int64_t getResponseCode() const;

		std::string getLogCategory() const;

		size_t getExceptionClassHash() const;

		~APIException() = default;
	};
}
