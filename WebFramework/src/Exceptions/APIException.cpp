#include "APIException.h"

using namespace std;

namespace framework::exceptions
{
	APIException::APIException(string_view errorMessage, int64_t responseCode, string_view logCategory, size_t exceptionClassHash) :
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

	string APIException::getLogCategory() const
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
