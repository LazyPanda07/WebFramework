#pragma once

#include <stdexcept>

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Errors in UTF8 convertion
		/// </summary>
		class StringConversionException : public std::runtime_error
		{
		public:
			StringConversionException();

			~StringConversionException() = default;
		};
	}
}
