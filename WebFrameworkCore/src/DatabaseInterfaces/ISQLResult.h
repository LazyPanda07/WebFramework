#pragma once

#include "ISQLValue.h"

namespace framework::interfaces
{
	// Also update CXX_API
	class ISQLResult
	{
	public:
		virtual size_t size() const = 0;

		virtual void iterate(void(*initBuffer)(size_t size, void* buffer), void (*callback)(const char** columnName, const void** columnValue, size_t size, size_t index, void* buffer), void* buffer) = 0;

		virtual ~ISQLResult() = default;
	};
}
