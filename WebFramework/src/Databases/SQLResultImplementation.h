#pragma once

#include "DatabaseInterfaces/ISQLResult.h"

#include "Framework/WebFrameworkPlatform.h"

#include "SQLResult.h"

namespace framework
{
	class WEB_FRAMEWORK_API SQLResultImplementation : public interfaces::ISQLResult
	{
	private:
		database::SQLResult result;

	public:
		SQLResultImplementation(database::SQLResult&& result);

		size_t size() const override;

		void iterate(void (initBuffer)(size_t size, void* buffer), void (*callback)(const char** columnName, const void** columnValue, size_t size, size_t index, void* buffer), void* buffer) override;

		~SQLResultImplementation() = default;
	};
}
