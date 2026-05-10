#pragma once

#include "Exceptions/BaseWebFrameworkException.h"

#include "Framework/WebFrameworkPlatform.h"

namespace framework::exceptions
{
	/// <summary>
	/// Wrong path to loadSource from .ini
	/// </summary>
	class CantLoadSourceException : public BaseWebFrameworkException
	{
	public:
		CantLoadSourceException(std::string_view source, std::string_view additionalInformation);

		~CantLoadSourceException() = default;
	};
}
