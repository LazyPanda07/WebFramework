#pragma once

#include "BaseWebFrameworkException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Wrong file path
		/// </summary>
		class WEB_FRAMEWORK_API FileDoesNotExistException : public BaseWebFrameworkException
		{
		public:
			FileDoesNotExistException(const std::string& filename);

			~FileDoesNotExistException() = default;
		};
	}
}
