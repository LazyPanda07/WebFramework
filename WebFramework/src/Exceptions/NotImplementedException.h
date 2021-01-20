#pragma once

#include "BaseExecutorException.h"

namespace framework
{
	namespace exceptions
	{
		/// <summary>
		/// <para>Default exception for all HTTP methods in BaseExecutor</para>
		/// <para>Server throws 400 error</para>
		/// </summary>
		class NotImplementedException : public BaseExecutorException
		{
		public:
			NotImplementedException();

			~NotImplementedException() = default;
		};
	}
}
