#pragma once

#include "Exceptions/BaseWebFrameworkException.h"

#include "Framework/WebFrameworkPlatform.h"

namespace framework::exceptions
{
	/// <summary>
	/// Wrong path to loadSource from .ini
	/// </summary>
	class WEB_FRAMEWORK_API CantLoadSourceException : public BaseWebFrameworkException
	{
	public:
		CantLoadSourceException(std::string_view source, std::string_view additionalInformation);

		~CantLoadSourceException() = default;
	};
}
