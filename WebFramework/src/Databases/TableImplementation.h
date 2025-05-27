#pragma once

#include "Databases/Interfaces/ITable.h"

#include "Framework/WebFrameworkPlatform.h"

#include "Table.h"

namespace framework
{
	class WEB_FRAMEWORK_API TableImplementation : public interfaces::ITable
	{
	private:
		database::Table* table;

	public:
		TableImplementation(database::Table* table);

		interfaces::ISQLResult* execute(const char* query, const interfaces::ISQLValue* values, size_t size) override;

		const char* getTableName() const override;

		~TableImplementation() = default;
	};
}
