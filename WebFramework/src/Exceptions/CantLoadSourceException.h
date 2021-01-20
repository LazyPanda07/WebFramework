#pragma once

#include <stdexcept>

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Wrong path to loadSource from .ini
		/// </summary>
		class CantLoadSourceException : public std::runtime_error
		{
		public:
			CantLoadSourceException(const std::string& source);

			~CantLoadSourceException() = default;
		};
	}
}
