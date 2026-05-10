#pragma once

#include "DatabaseInterfaces/ISQLResult.h"

#include "Framework/WebFrameworkPlatform.h"

#include "SqlResult.h"

namespace framework
{
	class SQLResultImplementation : public interfaces::ISQLResult
	{
	private:
		database::SqlResult result;

	public:
		SQLResultImplementation(database::SqlResult&& result);

		size_t size() const override;

		void iterate(void (initBuffer)(size_t size, void* buffer), void (*callback)(const char** columnName, const void** columnValue, size_t size, size_t index, void* buffer), void* buffer) override;

		~SQLResultImplementation() = default;
	};
}
