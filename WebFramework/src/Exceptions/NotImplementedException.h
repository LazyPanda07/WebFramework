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
		class WEB_FRAMEWORK_API NotImplementedException : public BadRequestException
		{
		public:
			NotImplementedException();

			~NotImplementedException() = default;
		};
	}
}
