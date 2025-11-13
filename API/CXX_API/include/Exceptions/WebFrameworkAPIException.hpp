#pragma once

#include <stdexcept>
#include <string>

#include "../HttpResponse.hpp"

namespace framework::exceptions
{
	class WebFrameworkAPIException : public std::runtime_error
	{
	private:
		std::string logCategory;
		ResponseCodes responseCode;

	public:
		WebFrameworkAPIException(std::string_view errorMessage, ResponseCodes responseCode, std::string_view logCategory = "");

		ResponseCodes getResponseCode() const;

		std::string_view getLogCategory() const;

		virtual ~WebFrameworkAPIException() = default;
	};

	inline WebFrameworkAPIException::WebFrameworkAPIException(std::string_view errorMessage, ResponseCodes responseCode, std::string_view logCategory) :
		runtime_error(errorMessage.data()),
		logCategory(logCategory),
		responseCode(responseCode)
	{

	}

	inline ResponseCodes WebFrameworkAPIException::getResponseCode() const
	{
		return responseCode;
	}

	inline std::string_view WebFrameworkAPIException::getLogCategory() const
	{
		return logCategory;
	}
}
