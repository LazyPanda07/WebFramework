#include "CSharpException.h"

#ifdef __WITH_DOT_NET_EXECUTORS__

namespace framework::exceptions
{
	CSharpException::CSharpException(std::string_view errorMessage, web::ResponseCodes responseCode, std::string_view logCategory) :
		BaseWebFrameworkException(errorMessage),
		responseCode(responseCode),
		logCategory(logCategory)
	{

	}

	web::ResponseCodes CSharpException::getResponseCode() const
	{
		return responseCode;
	}

	std::string_view CSharpException::getLogCategory() const
	{
		return logCategory;
	}
}

#endif
