#pragma once

#include <stdexcept>

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Syntax erros in .wfdp
		/// </summary>
		class DynamicPagesSyntaxException : public std::runtime_error
		{
		public:
			DynamicPagesSyntaxException(const std::string& syntaxError);

			~DynamicPagesSyntaxException() = default;
		};
	}
}
