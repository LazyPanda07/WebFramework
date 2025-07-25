#pragma once

#include "ISQLValue.h"

namespace framework::interfaces
{
	class ISQLResult
	{
	public:
		virtual size_t size() const = 0;

		virtual void iterate(void (*callback)(const char* columnName, const ISQLValue* columnValue, size_t index, size_t size, void* data), void* data) = 0;

		virtual ~ISQLResult() = default;
	};
}
