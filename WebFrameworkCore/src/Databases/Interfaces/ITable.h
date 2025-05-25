#pragma once

#include "ISQLResult.h"
#include "ISQLValue.h"

namespace framework::interfaces
{
	class WEB_FRAMEWORK_CORE_API ITable
	{
	public:
		virtual ISQLResult* execute(const char* query, const ISQLValue* values) = 0;

		virtual const char* getTableName() const = 0;

		virtual ~ITable() = default;
	};
}
