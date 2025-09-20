#include "APIException.h"

namespace framework::exceptions
{
	APIException::APIException(std::string_view errorMessage, int64_t responseCode, std::string_view logCategory, size_t exceptionClassHash) :
		BaseWebFrameworkException(errorMessage),
		logCategory(logCategory),
		responseCode(responseCode),
		exceptionClassHash(exceptionClassHash)
	{

	}

	int64_t APIException::getResponseCode() const
	{
		return responseCode;
	}

	std::string APIException::getLogCategory() const
	{
		return logCategory.empty() ?
			"LogAPI" :
			logCategory;
	}

	size_t APIException::getExceptionClassHash() const
	{
		return exceptionClassHash;
	}
}
