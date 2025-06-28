#include "APIException.h"

using namespace std;

namespace framework::exceptions
{
	APIException::APIException(string_view errorMessage, int64_t responseCode, string_view logCategory) :
		BaseWebFrameworkException(errorMessage),
		logCategory(logCategory),
		responseCode(responseCode)
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
}
