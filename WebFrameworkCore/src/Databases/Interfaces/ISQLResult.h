#pragma once

#include "WebFrameworkCoreConstants.h"

#include "ISQLValue.h"

namespace framework::interfaces
{
	class WEB_FRAMEWORK_CORE_API ISQLResult
	{
	public:
		virtual size_t size() const = 0;

		virtual void iterate(void (*callback)(const char* columnName, const ISQLValue* columnValue, size_t index, size_t size)) = 0;

		virtual ~ISQLResult() = default;
	};
}
