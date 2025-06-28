#pragma once

#include <Exceptions/BaseWebFrameworkException.h>

namespace framework::exceptions
{
	class APIException : public BaseWebFrameworkException
	{
	private:
		std::string logCategory;
		int64_t responseCode;

	public:
		APIException(std::string_view errorMessage, int64_t responseCode, std::string_view logCategory);

		int64_t getResponseCode() const;

		std::string getLogCategory() const;

		~APIException() = default;
	};
}
