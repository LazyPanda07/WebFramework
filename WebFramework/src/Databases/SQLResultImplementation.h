#pragma once

#include "Databases/Interfaces/ISQLResult.h"

#include "Framework/WebFrameworkPlatform.h"

#include "SQLResult.h"

namespace framework
{
	class WEB_FRAMEWORK_API SQLResultImplementation : interfaces::ISQLResult
	{
	private:
		database::SQLResult result;

	public:
		SQLResultImplementation(database::SQLResult&& result);

		size_t size() const override;

		void iterate(const char* columnName, interfaces::ISQLValue* columnValue, size_t index, size_t size) override;

		~SQLResultImplementation() = default;
	};
}
