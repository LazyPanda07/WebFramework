#pragma once

#include "BaseWebFrameworkException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Wrong path to loadSource from .ini
		/// </summary>
		class CantLoadSourceException : public BaseWebFrameworkException
		{
		public:
			CantLoadSourceException(std::string_view source);

			~CantLoadSourceException() = default;
		};
	}
}
