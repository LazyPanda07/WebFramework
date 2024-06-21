#pragma once

#include "BadRequestException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// Default exception for all HTTP methods in BaseExecutor
		/// <para>Server throws 400 error</para>
		/// </summary>
		class NotImplementedException : public BadRequestException
		{
		public:
			NotImplementedException(std::string_view className, std::string_view methodName);

			~NotImplementedException() = default;
		};
	}
}
