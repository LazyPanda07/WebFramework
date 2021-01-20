#pragma once

#include <stdexcept>

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Wrong file path
		/// </summary>
		class FileDoesNotExistException : public std::runtime_error
		{
		public:
			FileDoesNotExistException(const std::string& fileName);

			~FileDoesNotExistException() = default;
		};
	}
}
